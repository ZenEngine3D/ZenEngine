#include "zcCore.h"

namespace zcRes
{

GfxTexture2dProxy_DX11::GfxTexture2dProxy_DX11()
: mpTextureBuffer(NULL)
, mpTextureView(NULL)
{
}

GfxTexture2dProxy_DX11::~GfxTexture2dProxy_DX11()
{
	if( mpTextureBuffer )	
		mpTextureBuffer->Release();
	if( mpTextureView )		
		mpTextureView->Release();

	mpTextureBuffer = NULL;
	mpTextureView	= NULL;
}

bool GfxTexture2dProxy_DX11::Initialize(class GfxTexture2d& _Owner)
{
	const GfxTexture2d::ResDataRef& rResData = _Owner.GetResData();
	ZENAssert(rResData.IsValid());
	ZENDbgCode(mpOwner = &_Owner);

	D3D11_TEXTURE2D_DESC bufferDesc;
	D3D11_SHADER_RESOURCE_VIEW_DESC viewDesc;
	D3D11_SUBRESOURCE_DATA aInitData[16];
	ZeroMemory( &bufferDesc, sizeof(bufferDesc) );
	ZeroMemory( &viewDesc, sizeof(viewDesc) );
	ZeroMemory( aInitData, sizeof(aInitData) );		
	bool bIsDepth							= zcMgr::GfxRender.IsDepth(rResData->meFormat);
	bufferDesc.Width						= rResData->maMipData[0].mvDim.x;
	bufferDesc.Height						= rResData->maMipData[0].mvDim.y;
	bufferDesc.MipLevels					= rResData->maMipData.Count();
	bufferDesc.ArraySize					= 1;
	bufferDesc.Format						= zcMgr::GfxRender.ZenFormatToNative(rResData->meFormat);
	bufferDesc.SampleDesc.Count				= 1;
	bufferDesc.SampleDesc.Quality			= 0;
	bufferDesc.CPUAccessFlags				= 0;
	bufferDesc.MiscFlags					= 0;

	if( rResData->mCreationFlags.All(zenConst::keTexCreate_RenderTarget) )
	{			
		bufferDesc.Usage					= D3D11_USAGE_DEFAULT;
		bufferDesc.BindFlags				= bIsDepth ? D3D11_BIND_DEPTH_STENCIL : D3D11_BIND_SHADER_RESOURCE|D3D11_BIND_RENDER_TARGET;
	}
	else
	{
		ZENAssertMsg(!bIsDepth, "Only RenderTarget can be of type 'Depth'.");
		bufferDesc.Usage					= D3D11_USAGE_IMMUTABLE;
		bufferDesc.BindFlags				= D3D11_BIND_SHADER_RESOURCE;
	}

	bool bValidInitData(false);
	for(zUInt mipIdx(0); mipIdx<bufferDesc.MipLevels; ++mipIdx)
	{
		ZENAssert(mipIdx < ZENArrayCount(aInitData));			
		if( rResData->maMipData[mipIdx].maData.Count() > 0 )
		{
			bValidInitData					= true;
			aInitData[mipIdx].pSysMem		= rResData->maMipData[mipIdx].maData.First();
			aInitData[mipIdx].SysMemPitch	= zcExp::GetTextureBlockInfo(rResData->meFormat).muSize*rResData->maMipData[mipIdx].mvDim.x;
		}			
	}

	HRESULT ret = zcMgr::GfxRender.DX11GetDevice()->CreateTexture2D( &bufferDesc, bValidInitData ? aInitData : NULL, &mpTextureBuffer );
	if( SUCCEEDED(ret) && !bIsDepth)
	{
		viewDesc.Format						= bufferDesc.Format;
		viewDesc.ViewDimension				= D3D11_SRV_DIMENSION_TEXTURE2D;
		viewDesc.Texture2D.MostDetailedMip	= 0;
		viewDesc.Texture2D.MipLevels		= bufferDesc.MipLevels;
		ret = zcMgr::GfxRender.DX11GetDevice()->CreateShaderResourceView( mpTextureBuffer, &viewDesc, &mpTextureView );
	}

	return SUCCEEDED(ret);
}

}
