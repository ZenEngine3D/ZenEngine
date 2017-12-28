#include "zcCore.h"

namespace zcRes
{

GfxTexture2D_DX12::~GfxTexture2D_DX12()
{
}

bool GfxTexture2D_DX12::Initialize()
{
	const bool			bUploadData			= maMipData[0].maData.Count() > 0 && !mCreationFlags.Any(zenConst::keTexCreate_RenderTarget);
	D3D12_RESOURCE_DESC	TextureDesc			= {};	
	D3D12_CLEAR_VALUE	DefaultClearValue	= {};

	mResource.meState						= bUploadData ? D3D12_RESOURCE_STATE_COPY_DEST : D3D12_RESOURCE_STATE_COMMON;
	
	//----------------------------------------------------------------------------------------------
	// Allocate GPU memory for resource
	//----------------------------------------------------------------------------------------------
	bool bIsDepth					= zcMgr::GfxRender.IsDepth(meFormat);
	D3D12_CLEAR_VALUE* pClearValue	= nullptr;
	TextureDesc.Dimension			= D3D12_RESOURCE_DIMENSION_TEXTURE2D;	
	TextureDesc.Alignment			= 0;
	TextureDesc.Width				= maMipData[0].mvDim.x;
	TextureDesc.Height				= maMipData[0].mvDim.y;	
	TextureDesc.DepthOrArraySize	= 1;
	TextureDesc.MipLevels			= static_cast<UINT16>(maMipData.Count());
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
			mResource.meState						= D3D12_RESOURCE_STATE_DEPTH_WRITE;
			TextureDesc.Flags						|= D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL;
			DefaultClearValue.DepthStencil.Depth	= 1;
			DefaultClearValue.DepthStencil.Stencil	= 0;
		}
		else
		{
			mResource.meState						= D3D12_RESOURCE_STATE_RENDER_TARGET;
			TextureDesc.Flags						|= D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET;			
			DefaultClearValue.Color[0]				= DefaultClearValue.Color[1] = DefaultClearValue.Color[2] = 0;
			DefaultClearValue.Color[3]				= 1;
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
						mResource.meState,
						pClearValue,
						IID_PPV_ARGS(&mResource.mrResource) );
	if( FAILED(hr) )
		return nullptr;
	
	zSetGfxResourceName(mResource.mrResource, mResID, nullptr);
	

	//----------------------------------------------------------------------------------------------
	// Upload Texture data if present
	//! @todo support mipmaps
	//----------------------------------------------------------------------------------------------
	if( bUploadData )
	{		
		//for(zUInt mipIdx(0); mipIdx<TextureDesc.MipLevels; ++mipIdx)
		for(zUInt mipIdx(0); mipIdx<1; ++mipIdx)
		{			
			if( maMipData[mipIdx].maData.Count() > 0 )
			{
				zU8* pUploadData = reinterpret_cast<zU8*>(Lock());
				if( pUploadData )
				{
					zenMem::Copy(pUploadData, maMipData[mipIdx].maData.First(), maMipData[mipIdx].maData.SizeMem() );
					Unlock(zenGfx::zCommandList::GetFrameContext());					
				}
			}
		}
	}

	//----------------------------------------------------------------------------------------------
	// Create the Shader Resource View
	//----------------------------------------------------------------------------------------------
	// Describe and create a SRV for the texture.	
	D3D12_SHADER_RESOURCE_VIEW_DESC SrvDesc = {};
	SrvDesc.Format							= bIsDepth ? zcMgr::GfxRender.ZenFormatToDepthSRV(meFormat) : zcMgr::GfxRender.ZenFormatToNative(meFormat);
	SrvDesc.ViewDimension					= D3D12_SRV_DIMENSION_TEXTURE2D;
    SrvDesc.Shader4ComponentMapping			= D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	SrvDesc.Texture2D.MostDetailedMip		= 0;
    SrvDesc.Texture2D.MipLevels				= static_cast<UINT>(maMipData.Count());
    SrvDesc.Texture2D.PlaneSlice			= 0;
    SrvDesc.Texture2D.ResourceMinLODClamp	= 0;	
	zcMgr::GfxRender.GetDevice()->CreateShaderResourceView(mResource.mrResource.Get(), &SrvDesc, mResource.mView.GetCpu());

	return true;
}


void* GfxTexture2D_DX12::Lock()
{
	zenAssert(mResource.mrUpload.Get() == nullptr);

	UINT16					uMipCount		= 1; //maMipData.Count()
	D3D12_RESOURCE_DESC		TextureDesc		= mResource.mrResource->GetDesc();    
	D3D12_HEAP_PROPERTIES	UploadHeap		= {D3D12_HEAP_TYPE_UPLOAD, D3D12_CPU_PAGE_PROPERTY_UNKNOWN, D3D12_MEMORY_POOL_UNKNOWN, 1, 1};
	zcMgr::GfxRender.GetDevice()->GetCopyableFootprints(&TextureDesc, 0, uMipCount, 0, nullptr, nullptr, nullptr, &mResource.muUploadSize);
			
	D3D12_RESOURCE_DESC UploadDesc	= {};
	UploadDesc.Dimension			= D3D12_RESOURCE_DIMENSION_BUFFER;
	UploadDesc.Alignment			= 0;
	UploadDesc.Width				= mResource.muUploadSize;
	UploadDesc.Height				= 1;
	UploadDesc.DepthOrArraySize		= 1;
	UploadDesc.MipLevels			= uMipCount;
	UploadDesc.Format				= DXGI_FORMAT_UNKNOWN;
	UploadDesc.SampleDesc			= {1,0};
	UploadDesc.Layout				= D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
	UploadDesc.Flags				= D3D12_RESOURCE_FLAG_NONE;

	// Create the GPU upload buffer.
	HRESULT hr = zcMgr::GfxRender.GetDevice()->CreateCommittedResource(
						&UploadHeap,
						D3D12_HEAP_FLAG_NONE,
						&UploadDesc,
						D3D12_RESOURCE_STATE_GENERIC_READ,
						nullptr,
						IID_PPV_ARGS(&mResource.mrUpload));

	if( SUCCEEDED(hr) )
	{
		zSetGfxResourceName(mResource.mrUpload, mResID, L"Texture UploadData");
		void* pData(nullptr);
		hr = mResource.mrUpload->Map(0, NULL, &pData);
		if (SUCCEEDED(hr) )
			return pData;
	}

	return nullptr;
}

void GfxTexture2D_DX12::Unlock(const zenGfx::zCommandList& _rContext)
{
	zenAssert(mResource.mrUpload.Get() != nullptr);
	mResource.mrUpload->Unmap(0, NULL);
	zcRes::GfxTexture2DRef rTexture	= reinterpret_cast<zcRes::GfxTexture2D*>(this);
	zcGfx::CommandUpdateTexture_DX12::Add(_rContext, rTexture, maMipData[0].maData.SizeMem());
	mResource.mrUpload = nullptr;
}

}
