#include "zcCore.h"

namespace zcRes
{

	GfxWindowProxy_DX11::~GfxWindowProxy_DX11()
	{
		if( mDX11pSwapChain )
			mDX11pSwapChain->Release();	
	}

	bool GfxWindowProxy_DX11::Initialize(class GfxWindow& _Owner)
	{
		const GfxWindow::ResDataRef& rResData = _Owner.GetResData();
		ZENAssert(rResData.IsValid());
		ZENDbgCode(mpOwner = &_Owner);

		RECT rc;
		GetClientRect( rResData->mhWindow, &rc );
		mvSize					= zVec2U16(zU16(rc.right-rc.left), zU16(rc.bottom-rc.top));

		DXGI_SWAP_CHAIN_DESC swapDesc;
		ZeroMemory( &swapDesc, sizeof( swapDesc ) );
		swapDesc.BufferCount						= 2;
		swapDesc.BufferDesc.Width					= mvSize.x;
		swapDesc.BufferDesc.Height					= mvSize.y;
		swapDesc.BufferDesc.Format					= zcMgr::GfxRender.ZenFormatToNative(meBackbufferColorFormat);
		swapDesc.BufferDesc.RefreshRate.Numerator	= 60;
		swapDesc.BufferDesc.RefreshRate.Denominator	= 1;
		swapDesc.BufferUsage						= DXGI_USAGE_RENDER_TARGET_OUTPUT;
		swapDesc.BufferCount						= 2;
		swapDesc.OutputWindow						= rResData->mhWindow;
		swapDesc.SampleDesc.Count					= 1;
		swapDesc.SampleDesc.Quality					= 0;
		swapDesc.Windowed							= TRUE;
		swapDesc.SwapEffect							= DXGI_SWAP_EFFECT_DISCARD;
		
		IDXGIDevice*	pDXGIDevice;
		IDXGIAdapter*	pDXGIAdapter;
		IDXGIFactory*	pIDXGIFactory;		
		
		ID3D11Device*   DX11pDevice = zcMgr::GfxRender.DX11GetDevice();
		if( SUCCEEDED(DX11pDevice->QueryInterface(__uuidof(IDXGIDevice), (void **)&pDXGIDevice)) )
		{
			if( SUCCEEDED(pDXGIDevice->GetParent(__uuidof(IDXGIAdapter), (void **)&pDXGIAdapter)) )
			{
				if( SUCCEEDED(pDXGIAdapter->GetParent(__uuidof(IDXGIFactory), (void **)&pIDXGIFactory)) )
				{
					if( SUCCEEDED(pIDXGIFactory->CreateSwapChain( DX11pDevice, &swapDesc, &mDX11pSwapChain)) )
					{
						//! @todo can't allow access to owner in render thread, fix this
						//! @todo clean move code to resize and avoid duplicate						
						zEngineRef<GfxRenderTargetResData> rResData	= zenNewDefault GfxRenderTargetResData();
						bool bValid				= true;
						rResData->mResID		= zcMgr::Export.GetNewResourceID(zenConst::keResType_GfxRenderTarget);
						rResData->mbSRGB		= TRUE;
						rResData->meFormat		= meBackbufferColorFormat;
						rResData->mvDim			= mvSize;
						rResData->mpBackbuffer	= mDX11pSwapChain;
						for(zUInt idx(0); idx<ZENArrayCount(mrProxBackbufferColor) && bValid; ++idx)
						{
							rResData->muBackbufferId			= idx;
							GfxRenderTargetRef rBackbufferColor = GfxRenderTarget::RuntimeCreate(rResData).GetSafe();
							mrProxBackbufferColor[idx]			= rBackbufferColor;
							bValid								= mrProxBackbufferColor[idx].IsValid();
							mpOwner->SetBackbuffer(idx, rBackbufferColor); //! @todo urgent can't access game thread object here							
						}																	
						return bValid;
					}
				}
			}
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
		ZENAssert(mDX11pSwapChain);
		ZENAssertMsg(zcGfx::gWindowRender.IsValid()==false || zcGfx::gWindowRender->GetProxy() != this, "This method should only be called in ManagerBase::FrameStart()");

		if( !mvPendingResize.IsZero() && mvPendingResize != mvSize )
		{			
			for(zUInt idx(0); idx<ZENArrayCount(mrProxBackbufferColor); ++idx)
				mrProxBackbufferColor[idx]->GetProxy()->ReleaseBackbuffer();

			mDX11pSwapChain->ResizeBuffers(0, mvPendingResize.x, mvPendingResize.y, DXGI_FORMAT_UNKNOWN, 0);

			zEngineRef<GfxRenderTargetResData> rResData	= zenNewDefault GfxRenderTargetResData();
			mvSize										= mvPendingResize;
			rResData->mResID							= zcMgr::Export.GetNewResourceID( zenConst::keResType_GfxRenderTarget );
			rResData->mbSRGB							= TRUE;
			rResData->meFormat							= meBackbufferColorFormat;
			rResData->mvDim								= mvSize;
			rResData->mpBackbuffer						= mDX11pSwapChain;
			
			for (zUInt idx(0); idx<ZENArrayCount(mrProxBackbufferColor); ++idx)
			{
				rResData->muBackbufferId				= idx;
				GfxRenderTargetRef rBackbufferColor		= GfxRenderTarget::RuntimeCreate(rResData).GetSafe();
				mrProxBackbufferColor[idx]				= rBackbufferColor;
				mpOwner->SetBackbuffer(idx, rBackbufferColor); //! @todo urgent can't access game thread object here							
			}
		}		
		mvPendingResize.SetZero();
	}

}