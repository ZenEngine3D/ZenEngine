#include "zcCore.h"

namespace zcRes
{
	GfxWindowProxy_DX11::GfxWindowProxy_DX11()
	: mDX11pSwapChain(NULL)
	, mvPendingResize(0,0)
	{		
		mrProxBackbufferColor	= NULL;
		mrProxBackbufferDepth	= NULL;
		mvSize				= zVec2U16(0,0);
	}

	GfxWindowProxy_DX11::~GfxWindowProxy_DX11()
	{
		if( mDX11pSwapChain )
			mDX11pSwapChain->Release();	
	}

	bool GfxWindowProxy_DX11::Initialize(class GfxWindow& _Owner)
	{
		const GfxWindow::ExportDataRef& rExportData = _Owner.GetExportData();
		ZENAssert(rExportData.IsValid());
		ZENDbgCode(mpOwner = &_Owner);

		RECT rc;
		GetClientRect( rExportData->mhWindow, &rc );
		mvSize					= zVec2U16(zU16(rc.right-rc.left), zU16(rc.bottom-rc.top));
		meBackbufferColorFormat	= zenConst::keTexFormat_RGBA8;	//! @todo feature expose desired format in ExportData
		meBackbufferDepthFormat	= zenConst::keTexFormat_D24S8;

		DXGI_SWAP_CHAIN_DESC swapDesc;
		ZeroMemory( &swapDesc, sizeof( swapDesc ) );
		swapDesc.BufferCount						= 2;
		swapDesc.BufferDesc.Width					= mvSize.x;
		swapDesc.BufferDesc.Height					= mvSize.y;
		swapDesc.BufferDesc.Format					= EMgr::GfxRender.ZenFormatToNative(meBackbufferColorFormat);
		swapDesc.BufferDesc.RefreshRate.Numerator	= 60;
		swapDesc.BufferDesc.RefreshRate.Denominator	= 1;
		swapDesc.BufferUsage						= DXGI_USAGE_RENDER_TARGET_OUTPUT;
		swapDesc.OutputWindow						= rExportData->mhWindow;
		swapDesc.SampleDesc.Count					= 1;
		swapDesc.SampleDesc.Quality					= 0;
		swapDesc.Windowed							= TRUE;
		
		IDXGIDevice*	pDXGIDevice;
		IDXGIAdapter*	pDXGIAdapter;
		IDXGIFactory*	pIDXGIFactory;		
		
		ID3D11Device*   DX11pDevice = EMgr::GfxRender.DX11GetDevice();
		if( SUCCEEDED(DX11pDevice->QueryInterface(__uuidof(IDXGIDevice), (void **)&pDXGIDevice)) )
			if( SUCCEEDED(pDXGIDevice->GetParent(__uuidof(IDXGIAdapter), (void **)&pDXGIAdapter)) )
				if( SUCCEEDED(pDXGIAdapter->GetParent(__uuidof(IDXGIFactory), (void **)&pIDXGIFactory)) )
					if( SUCCEEDED(pIDXGIFactory->CreateSwapChain( DX11pDevice, &swapDesc, &mDX11pSwapChain)) )
					{
						//! @todo can't allow access to owner in renderthread, fix this
						//! @todo clean move code to resize and avoid duplicate
						GfxRenderTargetExportDataRef rExportData	= zenNewDefault GfxRenderTargetExportData();
						rExportData->mResID							= EMgr::Export.GetNewResourceID( zenConst::keResType_GfxRenderTarget );
						rExportData->mbSRGB							= TRUE;
						rExportData->meFormat						= meBackbufferColorFormat;
						rExportData->mvDim							= mvSize;
						rExportData->mpBackbuffer					= mDX11pSwapChain;
						
						GfxRenderTargetRef rBackbufferColor			= GfxRenderTarget::RuntimeCreate(rExportData);
						GfxRenderTargetRef rBackbufferDepth			= zcExp::CreateGfxRenderTarget( meBackbufferDepthFormat, mvSize);						
						GfxViewRef rView							= zcExp::CreateGfxView( rBackbufferColor.GetResID(), rBackbufferDepth.GetResID() );
						
						mrProxBackbufferColor						= rBackbufferColor->GetProxy();
						mrProxBackbufferDepth						= rBackbufferDepth->GetProxy();
						mrProxBackbufferView						= rView->GetProxy();
						_Owner.SetBackbuffer(rView);
						return mrProxBackbufferView.IsValid();
					}
		
		return false;;
	}

	//==================================================================================================
	//! @brief		Perform the resizing of the backbuffer
	//! @details	Called on frame start by ManagerRender to perform size changes requested earlier.
	//!				We should always be in a valid state for resizing when calling this 
	//!				(not in middle of a frame rendering)
	//--------------------------------------------------------------------------------------------------
	//! @return		
	//==================================================================================================
	void GfxWindowProxy_DX11::PerformResize()
	{
		zcRes::GfxWindowRef rWindowCur = EMgr::GfxRender.GetWindowCurrent();
		ZENAssert(mDX11pSwapChain);
		ZENAssertMsg(rWindowCur.IsValid()==false || rWindowCur->GetProxy() != this, "This method should only be called in ManagerBase::FrameStart()");

		if( !mvPendingResize.IsNull() && mvPendingResize != mvSize )
		{
			mvSize			= mvPendingResize;			
			mrProxBackbufferColor->ReleaseBackbuffer();
			mDX11pSwapChain->ResizeBuffers(0, mvPendingResize.x, mvPendingResize.y, DXGI_FORMAT_UNKNOWN, 0);

			GfxRenderTargetExportDataRef rExportData	= zenNewDefault GfxRenderTargetExportData();
			rExportData->mResID							= EMgr::Export.GetNewResourceID( zenConst::keResType_GfxRenderTarget );
			rExportData->mbSRGB							= TRUE;
			rExportData->meFormat						= meBackbufferColorFormat;
			rExportData->mvDim							= mvSize;
			rExportData->mpBackbuffer					= mDX11pSwapChain;
			
			GfxRenderTargetRef rBackbufferColor			= GfxRenderTarget::RuntimeCreate(rExportData);
			GfxRenderTargetRef rBackbufferDepth			= zcExp::CreateGfxRenderTarget( meBackbufferDepthFormat, mvSize);						
			GfxViewRef rView							= zcExp::CreateGfxView( rBackbufferColor.GetResID(), rBackbufferDepth.GetResID() );
						
			mrProxBackbufferColor						= rBackbufferColor->GetProxy();
			mrProxBackbufferDepth						= rBackbufferDepth->GetProxy();
			mrProxBackbufferView						= rView->GetProxy();
			mpOwner->SetBackbuffer(rView); //! @todo urgent can't access game thread object here
		}		
		mvPendingResize.SetNull();
	}

}