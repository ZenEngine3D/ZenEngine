#include "zcCore.h"

namespace zcRes
{

GfxTexture2dHAL_DX11::~GfxTexture2dHAL_DX11()
{
	if( mpTextureBuffer )	
		mpTextureBuffer->Release();
	if( mpTextureView )		
		mpTextureView->Release();

	mpTextureBuffer = nullptr;
	mpTextureView	= nullptr;
}

bool GfxTexture2dHAL_DX11::Initialize()
{
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

	HRESULT ret = zcMgr::GfxRender.DX11GetDevice()->CreateTexture2D( &bufferDesc, bValidInitData ? aInitData : nullptr, &mpTextureBuffer );
	if( SUCCEEDED(ret) && !bIsDepth)
	{
		viewDesc.Format						= bufferDesc.Format;
		viewDesc.ViewDimension				= D3D11_SRV_DIMENSION_TEXTURE2D;
		viewDesc.Texture2D.MostDetailedMip	= 0;
		viewDesc.Texture2D.MipLevels		= bufferDesc.MipLevels;
		ret									= zcMgr::GfxRender.DX11GetDevice()->CreateShaderResourceView( mpTextureBuffer, &viewDesc, &mpTextureView );
	}

	return SUCCEEDED(ret);
}

}
