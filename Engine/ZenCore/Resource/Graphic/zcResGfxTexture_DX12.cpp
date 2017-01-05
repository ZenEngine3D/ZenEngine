#include "zcCore.h"

namespace zcRes
{

//SF DX12
GfxTexture2dHAL_DX12::~GfxTexture2dHAL_DX12()
{
	mTextureView.Free();
}

bool GfxTexture2dHAL_DX12::Initialize()
{
#if ZEN_RENDERER_DX12
	D3D12_RESOURCE_DESC		TextureDesc = {};	
	D3D12_CLEAR_VALUE		DefaultClearValue = {};
	D3D12_RESOURCE_STATES	ResourceState = D3D12_RESOURCE_STATE_COMMON;

	bool bIsDepth					= zcMgr::GfxRender.IsDepth(meFormat);
	D3D12_CLEAR_VALUE* pClearValue	= nullptr;
	TextureDesc.Dimension			= D3D12_RESOURCE_DIMENSION_TEXTURE2D;	
	TextureDesc.Alignment			= 0;
	TextureDesc.Width				= maMipData[0].mvDim.x;
	TextureDesc.Height				= maMipData[0].mvDim.y;	
	TextureDesc.DepthOrArraySize	= 1;
	TextureDesc.MipLevels			= static_cast<UINT>(maMipData.Count());
	TextureDesc.Format				= zcMgr::GfxRender.ZenFormatToTypeless(meFormat);
	TextureDesc.SampleDesc.Count	= 1;
	TextureDesc.SampleDesc.Quality	= 0;
	TextureDesc.Layout				= D3D12_TEXTURE_LAYOUT_UNKNOWN;
	TextureDesc.Flags				= D3D12_RESOURCE_FLAG_NONE;
	
	if( mCreationFlags.Any(zenConst::keTexCreate_RenderTarget) )
	{
		pClearValue					= &DefaultClearValue;
		DefaultClearValue.Format	= zcMgr::GfxRender.ZenFormatToNative(meFormat);
		if( bIsDepth )
		{
			ResourceState							= D3D12_RESOURCE_STATE_DEPTH_WRITE;
			TextureDesc.Flags						|= D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL;
			DefaultClearValue.DepthStencil.Depth	= 1;
			DefaultClearValue.DepthStencil.Stencil	= 0;
		}
		else
		{
			ResourceState				= D3D12_RESOURCE_STATE_RENDER_TARGET;
			TextureDesc.Flags			|= D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET;			
			DefaultClearValue.Color[0]	= DefaultClearValue.Color[1] = DefaultClearValue.Color[2] = 0;
			DefaultClearValue.Color[3]	= 1;
		}
	}	

	D3D12_HEAP_PROPERTIES ResourceHeap	= {};
	ResourceHeap.Type					= D3D12_HEAP_TYPE_DEFAULT;
    ResourceHeap.CPUPageProperty		= D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
    ResourceHeap.MemoryPoolPreference	= D3D12_MEMORY_POOL_UNKNOWN;
    ResourceHeap.CreationNodeMask		= 0;
    ResourceHeap.VisibleNodeMask		= 0;
	HRESULT hr = zcMgr::GfxRender.GetDevice()->CreateCommittedResource(
						&ResourceHeap,
						D3D12_HEAP_FLAG_NONE,
						&TextureDesc,
						ResourceState,
						pClearValue,
						IID_PPV_ARGS(&mrTextureResource) );
	if( FAILED(hr) )
		return nullptr;
#if 0 //Upload texture data
	// Note: ComPtr's are CPU objects but this resource needs to stay in scope until
	// the command list that references it has finished executing on the GPU.
	// We will flush the GPU at the end of this method to ensure the resource is not
	// prematurely destroyed.
	DirectXComRef<ID3D12Resource> rTextureUploadHeap;
	UINT64 RequiredSize					= 0;
	D3D12_RESOURCE_DESC TextureDesc		= mrTextureResource->GetDesc();    
	D3D12_HEAP_PROPERTIES UploadHeap	= {D3D12_HEAP_TYPE_UPLOAD, D3D12_CPU_PAGE_PROPERTY_UNKNOWN, D3D12_MEMORY_POOL_UNKNOWN, 1, 1};
    zcMgr::GfxRender.GetDevice()->GetCopyableFootprints(&TextureDesc, 0, maMipData.Count(), 0, nullptr, nullptr, nullptr, &RequiredSize);

	D3D12_RESOURCE_DESC UploadDesc	= {};
	UploadDesc.Dimension			= D3D12_RESOURCE_DIMENSION_BUFFER;
    UploadDesc.Alignment			= 0;
    UploadDesc.Width				= RequiredSize;
    UploadDesc.Height				= 1;
    UploadDesc.DepthOrArraySize		= 1;
    UploadDesc.MipLevels			= maMipData.Count();
    UploadDesc.Format				= DXGI_FORMAT_UNKNOWN;
	UploadDesc.SampleDesc			= {1,0};
    UploadDesc.Layout				= D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
    UploadDesc.Flags				= D3D12_RESOURCE_FLAG_NONE;

	// Create the GPU upload buffer.
	hr = zcMgr::GfxRender.GetDevice()->CreateCommittedResource(
						&UploadHeap,
						D3D12_HEAP_FLAG_NONE,
						&UploadDesc,
						D3D12_RESOURCE_STATE_GENERIC_READ,
						nullptr,
						IID_PPV_ARGS(&rTextureUploadHeap)));

	
	D3D12_SUBRESOURCE_DATA textureData	= {};
	textureData.pData					= &texture[0];
	textureData.RowPitch				= TextureWidth * TexturePixelSize;
	textureData.SlicePitch				= textureData.RowPitch * TextureHeight;

	UpdateSubresources(m_commandList.Get(), m_texture.Get(), textureUploadHeap.Get(), 0, 0, 1, &textureData);
	m_commandList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(m_texture.Get(), D3D12_RESOURCE_STATE_COPY_DEST, D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE));
#endif

	// Describe and create a SRV for the texture.	
	D3D12_SHADER_RESOURCE_VIEW_DESC SrvDesc = {};
	SrvDesc.Format							= bIsDepth ? zcMgr::GfxRender.ZenFormatToDepthSRV(meFormat) : zcMgr::GfxRender.ZenFormatToNative(meFormat);
	SrvDesc.ViewDimension					= D3D12_SRV_DIMENSION_TEXTURE2D;
    SrvDesc.Shader4ComponentMapping			= D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	SrvDesc.Texture2D.MostDetailedMip		= 0;
    SrvDesc.Texture2D.MipLevels				= static_cast<UINT>(maMipData.Count());
    SrvDesc.Texture2D.PlaneSlice			= 0;
    SrvDesc.Texture2D.ResourceMinLODClamp	= 0;
	mTextureView							= zcGfx::DescriptorSRV_UAV_CBV::Allocate();
	zcMgr::GfxRender.GetDevice()->CreateShaderResourceView(mrTextureResource.Get(), &SrvDesc, mTextureView.GetHandle());

	return true;
#else
	D3D11_TEXTURE2D_DESC			bufferDesc;		ZeroMemory( &bufferDesc, sizeof(bufferDesc) );
	D3D11_SHADER_RESOURCE_VIEW_DESC viewDesc;		ZeroMemory( &viewDesc, sizeof(viewDesc) );
	D3D11_SUBRESOURCE_DATA			aInitData[16];	ZeroMemory( aInitData, sizeof(aInitData) );
	
	bool bIsDepth							= zcMgr::GfxRender.IsDepth(meFormat);
	bufferDesc.Width						= maMipData[0].mvDim.x;
	bufferDesc.Height						= maMipData[0].mvDim.y;
	bufferDesc.MipLevels					= static_cast<UINT>(maMipData.Count());
	bufferDesc.ArraySize					= 1;
	bufferDesc.Format						= zcMgr::GfxRender.ZenFormatToNative(meFormat);
	bufferDesc.SampleDesc.Count				= 1;
	bufferDesc.SampleDesc.Quality			= 0;
	bufferDesc.CPUAccessFlags				= 0;
	bufferDesc.MiscFlags					= 0;

	if( mCreationFlags.All(zenConst::keTexCreate_RenderTarget) )
	{			
		bufferDesc.Usage					= D3D11_USAGE_DEFAULT;
		bufferDesc.BindFlags				= bIsDepth ? D3D11_BIND_DEPTH_STENCIL : D3D11_BIND_SHADER_RESOURCE|D3D11_BIND_RENDER_TARGET;
	}
	else
	{
		zenAssertMsg(!bIsDepth, "Only RenderTarget can be of type 'Depth'.");
		bufferDesc.Usage					= D3D11_USAGE_IMMUTABLE;
		bufferDesc.BindFlags				= D3D11_BIND_SHADER_RESOURCE;
	}

	bool bValidInitData(false);
	for(zUInt mipIdx(0); mipIdx<bufferDesc.MipLevels; ++mipIdx)
	{
		zenAssert(mipIdx < zenArrayCount(aInitData));			
		if( maMipData[mipIdx].maData.Count() > 0 )
		{
			bValidInitData					= true;
			aInitData[mipIdx].pSysMem		= maMipData[mipIdx].maData.First();
			aInitData[mipIdx].SysMemPitch	= zcExp::GetTextureBlockInfo(meFormat).muSize*maMipData[mipIdx].mvDim.x;
		}			
	}

	HRESULT ret = zcMgr::GfxRender.DX12GetDevice()->CreateTexture2D( &bufferDesc, bValidInitData ? aInitData : nullptr, &mpTextureBuffer );
	if( SUCCEEDED(ret) && !bIsDepth)
	{
		viewDesc.Format						= bufferDesc.Format;
		viewDesc.ViewDimension				= D3D11_SRV_DIMENSION_TEXTURE2D;
		viewDesc.Texture2D.MostDetailedMip	= 0;
		viewDesc.Texture2D.MipLevels		= bufferDesc.MipLevels;
		ret									= zcMgr::GfxRender.DX12GetDevice()->CreateShaderResourceView( mpTextureBuffer, &viewDesc, &mpTextureView );
	}

	return SUCCEEDED(ret);
#endif
}

}
