#include "zcCore.h"

namespace zcRes
{

GfxTarget2D_DX11::~GfxTarget2D_DX11()
{
	if(mpTargetColorView)
		mpTargetColorView->Release();			

	if(mpTargetDepthView)
		mpTargetDepthView->Release();
			
	mpTargetDepthView		= nullptr;
	mpTargetColorView		= nullptr;
}

//==================================================================================================
//! @brief		Create a RenderTarget from the backbuffer, skipping the export process
//==================================================================================================
GfxTarget2DRef GfxTarget2D_DX11::RuntimeCreate(IDXGISwapChain& _Swapchain, zenConst::eTextureFormat _eTexFormat, zUInt _uBufferId)
{		
	ID3D11Texture2D*		pTexture(nullptr);
	GfxTarget2DRef			rResource(nullptr);

	_Swapchain.GetBuffer( 0, __uuidof(ID3D11Texture2D), (LPVOID*)&pTexture );
	if( pTexture )
	{
		static zenMem::zAllocatorPool sMemPool("Pool GfxTarget2D", sizeof(GfxTarget2D), 128, 128);
		rResource					= zenNew(&sMemPool) GfxTarget2D();		

		//todo clean create texture from swapchain
		D3D11_TEXTURE2D_DESC TextureDesc;
		pTexture->GetDesc(&TextureDesc);						
		rResource.HAL()->meFormat	= _eTexFormat;
		rResource.HAL()->mvDim		= zVec2U16((zU16)TextureDesc.Width, (zU16)TextureDesc.Height);			
		bool bSuccess				= rResource.HAL()->InitializeCommon(*pTexture);
		pTexture->Release();
		if( !bSuccess )
			return nullptr;
			
		RuntimeCreateFinalize( *rResource.HAL(), zenConst::keResType_GfxTarget2D );
	}
		
	return rResource;
}

//==================================================================================================
//==================================================================================================
bool GfxTarget2D_DX11::Initialize()
{
	GfxTarget2DRef		rRenderTarget		= (GfxTarget2D*)this;
	GfxTexture2DRef		rTexture			= rRenderTarget->GetTexture2D();
	return InitializeCommon(*rTexture.HAL()->mpTextureBuffer);
}

//==================================================================================================
//! @brief		Creates the resource view on the render target
//==================================================================================================
bool GfxTarget2D_DX11::InitializeCommon(ID3D11Texture2D& _Texture)
{
	HRESULT hr(S_FALSE);
	if( zcMgr::GfxRender.IsDepth(meFormat) )
	{
		D3D11_DEPTH_STENCIL_VIEW_DESC descDSV;
		descDSV.Format				= zcMgr::GfxRender.ZenFormatToNative(meFormat);
		descDSV.ViewDimension		= D3D11_DSV_DIMENSION_TEXTURE2D;
		descDSV.Flags				= 0;
		descDSV.Texture2D.MipSlice	= 0;
		hr = zcMgr::GfxRender.GetDevice()->CreateDepthStencilView( &_Texture, &descDSV, &mpTargetDepthView );
	}
	else
	{
		hr = zcMgr::GfxRender.GetDevice()->CreateRenderTargetView( &_Texture, nullptr, &mpTargetColorView );				
	}
	return SUCCEEDED(hr);
}

//==================================================================================================
//! @brief		Free up the reference to swapchain backbuffer
//! @details	Called GfxWindow_DX11::PerformResize to remove all reference to swapchain, before
//!				performing a resize on it
//--------------------------------------------------------------------------------------------------
//! @return		
//==================================================================================================
void GfxTarget2D_DX11::ReleaseBackbuffer()
{
	if( mpTargetColorView )
		mpTargetColorView->Release();
	mpTargetColorView = nullptr;
}

}
