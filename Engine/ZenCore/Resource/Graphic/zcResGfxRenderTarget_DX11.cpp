#include "zcCore.h"

namespace zcRes
{
	GfxRenderTargetProxy_DX11::GfxRenderTargetProxy_DX11()
	: mpTargetColorView(nullptr)
	, mpTargetDepthView(nullptr)
	, mbNeedResolve(false)
	{	
	}

	GfxRenderTargetProxy_DX11::~GfxRenderTargetProxy_DX11()
	{		
		if(mpTargetColorView)
			mpTargetColorView->Release();			

		if(mpTargetDepthView)
			mpTargetDepthView->Release();
			
		mpTargetDepthView		= nullptr;
		mpTargetColorView		= nullptr;
	}

	bool GfxRenderTargetProxy_DX11::Initialize(class GfxRenderTarget& _Owner)
	{
		HRESULT hr(S_FALSE);
		ZENDbgCode(mpOwner = &_Owner);
		const GfxRenderTargetResDataRef& rResData = _Owner.GetResData();
		ZENAssert(rResData.IsValid());
		ID3D11Texture2D* pTexture(NULL);

		mrProxParentTexture		= _Owner.GetTexture2D().IsValid() ? _Owner.GetTexture2D() : NULL;
		meFormat				= rResData->meFormat;
		mvDim					= rResData->mvDim;

		if( rResData->mpBackbuffer )
		{
			rResData->mpBackbuffer->GetBuffer( 0, __uuidof(ID3D11Texture2D), (LPVOID*)&pTexture );
		}
		else if(mrProxParentTexture.IsValid() )
		{
			pTexture = mrProxParentTexture->GetProxy()->mpTextureBuffer;
		}

		if( pTexture )
		{
			if( zcMgr::GfxRender.IsDepth(meFormat) )
			{
				D3D11_DEPTH_STENCIL_VIEW_DESC descDSV;
				descDSV.Format				= zcMgr::GfxRender.ZenFormatToNative(meFormat);
				descDSV.ViewDimension		= D3D11_DSV_DIMENSION_TEXTURE2D;
				descDSV.Flags				= 0;
				descDSV.Texture2D.MipSlice	= 0;
				hr = zcMgr::GfxRender.DX11GetDevice()->CreateDepthStencilView( pTexture, &descDSV, &mpTargetDepthView );
			}
			else
			{
				hr = zcMgr::GfxRender.DX11GetDevice()->CreateRenderTargetView( pTexture, NULL, &mpTargetColorView );				
			}
			
			if( rResData->mpBackbuffer )
				pTexture->Release();
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
		mpTargetColorView = nullptr;
	}

	void GfxRenderTargetProxy_DX11::Clear(const zVec4F& _vRGBA)
	{
		ZENAssertMsg( !IsDepth(), "Trying to clear a depth rendertarget as color.");		
		zcMgr::GfxRender.DX11GetDeviceContext()->ClearRenderTargetView( mpTargetColorView, _vRGBA.xyzw );	
	}

	void GfxRenderTargetProxy_DX11::Clear(float _fDepth, zU8 _uStencil, bool _bClearDepth, bool _bClearStencil)
	{
		ZENAssertMsg( IsDepth(), "Trying to clear a color rendertarget as depth.");
		UINT ClearFlags  = _bClearDepth		? D3D11_CLEAR_DEPTH		: 0;
		ClearFlags		|= _bClearStencil	? D3D11_CLEAR_STENCIL	: 0; 
		zcMgr::GfxRender.DX11GetDeviceContext()->ClearDepthStencilView( mpTargetDepthView, ClearFlags, _fDepth, _uStencil );
	}
}