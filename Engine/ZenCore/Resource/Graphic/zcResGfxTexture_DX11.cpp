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
	const GfxTexture2d::ExportDataRef& rExportData = _Owner.GetExportData();
	ZENAssert(rExportData.IsValid());
	ZENDbgCode(mpOwner = &_Owner);

	D3D11_TEXTURE2D_DESC bufferDesc;
	D3D11_SHADER_RESOURCE_VIEW_DESC viewDesc;
	D3D11_SUBRESOURCE_DATA aInitData[16];
	ZeroMemory( &bufferDesc, sizeof(bufferDesc) );
	ZeroMemory( &viewDesc, sizeof(viewDesc) );
	ZeroMemory( aInitData, sizeof(aInitData) );		
	bool bIsDepth							= EMgr::GfxRender.IsDepth(rExportData->meFormat);
	bufferDesc.Width						= rExportData->maMipData[0].mvDim.x;
	bufferDesc.Height						= rExportData->maMipData[0].mvDim.y;
	bufferDesc.MipLevels					= rExportData->maMipData.Count();
	bufferDesc.ArraySize					= 1;
	bufferDesc.Format						= EMgr::GfxRender.ZenFormatToNative(rExportData->meFormat);
	bufferDesc.SampleDesc.Count				= 1;
	bufferDesc.SampleDesc.Quality			= 0;
	bufferDesc.CPUAccessFlags				= 0;
	bufferDesc.MiscFlags					= 0;

	if( rExportData->mCreationFlags.All(zenConst::keTexCreate_RenderTarget) )
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
		if( rExportData->maMipData[mipIdx].maData.Count() > 0 )
		{
			bValidInitData					= true;
			aInitData[mipIdx].pSysMem		= rExportData->maMipData[mipIdx].maData.First();
			aInitData[mipIdx].SysMemPitch	= zcExp::GetTextureBlockInfo(rExportData->meFormat).muSize*rExportData->maMipData[mipIdx].mvDim.x;
		}			
	}

	HRESULT ret = EMgr::GfxRender.DX11GetDevice()->CreateTexture2D( &bufferDesc, bValidInitData ? aInitData : NULL, &mpTextureBuffer );
	if( SUCCEEDED(ret) && !bIsDepth)
	{
		viewDesc.Format						= bufferDesc.Format;
		viewDesc.ViewDimension				= D3D11_SRV_DIMENSION_TEXTURE2D;
		viewDesc.Texture2D.MostDetailedMip	= 0;
		viewDesc.Texture2D.MipLevels		= bufferDesc.MipLevels;
		ret = EMgr::GfxRender.DX11GetDevice()->CreateShaderResourceView( mpTextureBuffer, &viewDesc, &mpTextureView );
	}

	return SUCCEEDED(ret);
}

}
