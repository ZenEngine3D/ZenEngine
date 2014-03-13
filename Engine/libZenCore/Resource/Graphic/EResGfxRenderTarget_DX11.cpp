#include "libZenCore.h"

namespace ERes
{
	GfxRenderTarget_DX11::GfxRenderTarget_DX11()
	: mpSwapchainBackbuffer(NULL)
	, mbNeedResolve(false)
	{
		mInstanceInfo.mpTargetColorView	= NULL;
		mInstanceInfo.mpTargetDepthView	= NULL;
		mInstanceInfo.mrTargetTexture	= zResID();		
	}

	GfxRenderTarget_DX11::~GfxRenderTarget_DX11()
	{		
		if(mpSwapchainBackbuffer)
		{
			mpSwapchainBackbuffer->Release();
			mpSwapchainBackbuffer = NULL;
		}
		if(mInstanceInfo.mpTargetColorView)
		{
			mInstanceInfo.mpTargetColorView->Release();
			mInstanceInfo.mpTargetColorView = NULL;
		}
		if(mInstanceInfo.mpTargetDepthView)
		{
			mInstanceInfo.mpTargetDepthView->Release();
			mInstanceInfo.mpTargetDepthView = NULL;
		}
	}

	bool GfxRenderTarget_DX11::ResourceInit()
	{
		HRESULT hr(S_FALSE);

		mInstanceInfo.mrTargetTexture = EExp::CreateGfxTexture2D(mInstanceInfo.meFormat, zenFlagResTexCreate(zenConst::keTexCreate_RenderTarget), mInstanceInfo.mvDim );
		if( mInstanceInfo.mrTargetTexture.IsValid() )
		{
			if( EMgr::GfxRender.IsDepth(mInstanceInfo.meFormat) )
			{
				D3D11_DEPTH_STENCIL_VIEW_DESC descDSV;
				descDSV.Format				= EMgr::GfxRender.AWFormatToNative(mInstanceInfo.meFormat);
				descDSV.ViewDimension		= D3D11_DSV_DIMENSION_TEXTURE2D;
				descDSV.Flags				= 0;
				descDSV.Texture2D.MipSlice	= 0;
				hr = EMgr::GfxRender.DX11GetDevice()->CreateDepthStencilView( mInstanceInfo.mrTargetTexture->Get().mpTextureBuffer, &descDSV, &mInstanceInfo.mpTargetDepthView );
			}
			else
			{
				hr = EMgr::GfxRender.DX11GetDevice()->CreateRenderTargetView( mInstanceInfo.mrTargetTexture->Get().mpTextureBuffer, NULL, &mInstanceInfo.mpTargetColorView );			
			}
		}
		return SUCCEEDED(hr);
	}

	//=================================================================================================
	//! @brief		Create a render target from the backbuffer
	//! @details	Rare case where resource is created directly instead of using export pipeline.
	//!				Takes reference to DirectX11 backbuffer, and create a RenderTarget ressource 
	//!				from it, for us to use.
	//-------------------------------------------------------------------------------------------------
	//! @param _pSwapchain -	DirectX backbuffer object
	//! @param _eFormat -		Backbuffer texture format
	//! @param _vDim -			Dimensions of the rendertarget
	//=================================================================================================
	//SF TODO Cleanup this recource creation process 
	GfxRenderTargetRef GfxRenderTarget_DX11::CreateFromBackuffer(IDXGISwapChain* _pSwapchain, zenConst::eTextureFormat _eFormat, const zVec2U16& _vDim)
	{
		ID3D11Texture2D*		pColorTexture(NULL);
		ID3D11RenderTargetView*	pColorView;
		GfxRenderTarget_DX11*		pBackbufferTarget(NULL);
		if( SUCCEEDED(_pSwapchain->GetBuffer( 0, __uuidof(ID3D11Texture2D), (LPVOID*)&pColorTexture )) )
		{
			if( SUCCEEDED(EMgr::GfxRender.DX11GetDevice()->CreateRenderTargetView( pColorTexture, NULL, &pColorView )) )
			{
				bool bUnused;
				pBackbufferTarget									= zenNewDefault GfxRenderTarget_DX11;
				pBackbufferTarget->mResID							= EMgr::Export.GetNewResourceID( zenConst::kAWCurrentPlatformGfx, zenConst::keResType_GfxRenderTarget, zenConst::keResSource_Runtime, NULL, bUnused );
				pBackbufferTarget->mInstanceInfo.mResID				= pBackbufferTarget->mResID;
				pBackbufferTarget->mInstanceInfo.mpTargetColorView	= pColorView;
				pBackbufferTarget->mInstanceInfo.mrTargetTexture	= NULL;
				pBackbufferTarget->mInstanceInfo.meFormat			= _eFormat;
				pBackbufferTarget->mInstanceInfo.mvDim				= _vDim;
				pBackbufferTarget->mInstanceInfo.mbSRGB				= TRUE;
				pBackbufferTarget->mpSwapchainBackbuffer			= pColorTexture;
				pBackbufferTarget->ResourceCreateRuntime();
			}
			else
				pColorTexture->Release();
		}		
		return pBackbufferTarget;
	}

	GfxTexture2DRef GfxRenderTarget_DX11::GetTexture2D()
	{
		ZENAssertMsg( mInstanceInfo.mpTargetColorView, "Only color RenderTarget can be used as texture" );
		ZENAssertMsg( mInstanceInfo.mrTargetTexture.IsValid(), "Access to backbuffer texture is unsupported.")
		if( mbNeedResolve )
		{
			//Multisample resolve here
		}
		mbNeedResolve = false;
		return mInstanceInfo.mrTargetTexture;
	}

	//==================================================================================================
	//! @brief		Free up the reference to swapchain backbuffer
	//! @details	Called GfxWindow_DX11::PerformResize to remove all reference to swapchain, before
	//!				performing a resize on it
	//--------------------------------------------------------------------------------------------------
	//! @return		
	//==================================================================================================
	void GfxRenderTarget_DX11::ReleaseBackbuffer()
	{
		ZENAssert( mInstanceInfo.mpTargetColorView && !mInstanceInfo.mrTargetTexture.IsValid() );	//Should only be called by GfxWindow_DX11::PerformResize
		mInstanceInfo.mpTargetColorView->Release();
		mInstanceInfo.mpTargetColorView = NULL;
		mpSwapchainBackbuffer->Release();
		mpSwapchainBackbuffer = NULL;
	}

	void GfxRenderTarget_DX11::Clear(const zVec4F& _vRGBA)
	{
		ZENAssertMsg( !IsDepth(), "Trying to clear a depth rendertarget as color.");		
		EMgr::GfxRender.DX11GetDeviceContext()->ClearRenderTargetView( mInstanceInfo.mpTargetColorView, _vRGBA.xyzw );	
	}

	void GfxRenderTarget_DX11::Clear(float _fDepth, zU8 _uStencil, bool _bClearDepth, bool _bClearStencil)
	{
		ZENAssertMsg( IsDepth(), "Trying to clear a color rendertarget as depth.");
		UINT ClearFlags  = _bClearDepth		? D3D11_CLEAR_DEPTH		: 0;
		ClearFlags		|= _bClearStencil	? D3D11_CLEAR_STENCIL	: 0; 
		EMgr::GfxRender.DX11GetDeviceContext()->ClearDepthStencilView( mInstanceInfo.mpTargetDepthView, ClearFlags, _fDepth, _uStencil );
	}
}