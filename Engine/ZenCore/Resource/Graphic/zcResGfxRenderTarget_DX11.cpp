#include "zcCore.h"

namespace zcRes
{
	GfxRenderTargetProxy_DX11::GfxRenderTargetProxy_DX11()
	: mpSwapchainBackbuffer(NULL)
	, mpTargetColorView(NULL)
	, mpTargetDepthView(NULL)
	, mbNeedResolve(false)
	{	
	}

	GfxRenderTargetProxy_DX11::~GfxRenderTargetProxy_DX11()
	{		
		if(mpSwapchainBackbuffer)
			mpSwapchainBackbuffer->Release();
			
		if(mpTargetColorView)
			mpTargetColorView->Release();			

		if(mpTargetDepthView)
			mpTargetDepthView->Release();
			
		mpTargetDepthView		= NULL;
		mpTargetColorView		= NULL;
		mpSwapchainBackbuffer	= NULL;
	}

	bool GfxRenderTargetProxy_DX11::Initialize(class GfxRenderTarget& _Owner)
	{
		HRESULT hr(S_FALSE);
		ZENDbgCode(mpOwner = &_Owner);
		const GfxRenderTargetExportDataRef& rExportData	= _Owner.GetExportData();
		ZENAssert(rExportData.IsValid());
		ID3D11Texture2D* pTexture(NULL);

		mrProxParentTexture	= _Owner.GetTexture2D().IsValid() ? _Owner.GetTexture2D()->GetProxy() : NULL;
		meFormat				= rExportData->meFormat;
		mvDim					= rExportData->mvDim;

		if( rExportData->mpBackbuffer )
			rExportData->mpBackbuffer->GetBuffer( 0, __uuidof(ID3D11Texture2D), (LPVOID*)&pTexture );
		else if(mrProxParentTexture.IsValid() )
			pTexture = mrProxParentTexture->mpTextureBuffer;

		if( pTexture )
		{
			if( EMgr::GfxRender.IsDepth(meFormat) )
			{
				D3D11_DEPTH_STENCIL_VIEW_DESC descDSV;
				descDSV.Format				= EMgr::GfxRender.ZenFormatToNative(meFormat);
				descDSV.ViewDimension		= D3D11_DSV_DIMENSION_TEXTURE2D;
				descDSV.Flags				= 0;
				descDSV.Texture2D.MipSlice	= 0;
				hr = EMgr::GfxRender.DX11GetDevice()->CreateDepthStencilView( pTexture, &descDSV, &mpTargetDepthView );
			}
			else
			{
				hr = EMgr::GfxRender.DX11GetDevice()->CreateRenderTargetView( pTexture, NULL, &mpTargetColorView );			
			}
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
	void GfxRenderTargetProxy_DX11::ReleaseBackbuffer()
	{
		if( mpTargetColorView )
			mpTargetColorView->Release();
		
		if(mpSwapchainBackbuffer)
			mpSwapchainBackbuffer->Release();
		
		mpSwapchainBackbuffer	= NULL;
		mpTargetColorView		= NULL;
	}

	void GfxRenderTargetProxy_DX11::Clear(const zVec4F& _vRGBA)
	{
		ZENAssertMsg( !IsDepth(), "Trying to clear a depth rendertarget as color.");		
		EMgr::GfxRender.DX11GetDeviceContext()->ClearRenderTargetView( mpTargetColorView, _vRGBA.xyzw );	
	}

	void GfxRenderTargetProxy_DX11::Clear(float _fDepth, zU8 _uStencil, bool _bClearDepth, bool _bClearStencil)
	{
		ZENAssertMsg( IsDepth(), "Trying to clear a color rendertarget as depth.");
		UINT ClearFlags  = _bClearDepth		? D3D11_CLEAR_DEPTH		: 0;
		ClearFlags		|= _bClearStencil	? D3D11_CLEAR_STENCIL	: 0; 
		EMgr::GfxRender.DX11GetDeviceContext()->ClearDepthStencilView( mpTargetDepthView, ClearFlags, _fDepth, _uStencil );
	}
}