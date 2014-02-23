#include "libZenCore.h"

namespace ERes
{
	GfxWindow_DX11::GfxWindow_DX11()
	: mvPendingResize(0,0)
	{
		mInstanceInfo.mDX11pSwapChain	= NULL;
		mInstanceInfo.mrBackbufferColor	= NULL;
		mInstanceInfo.mrBackbufferDepth	= NULL;
		mInstanceInfo.mvSize			= awVec2U16(0,0);
	}

	GfxWindow_DX11::~GfxWindow_DX11()
	{
		if( mInstanceInfo.mDX11pSwapChain )
			mInstanceInfo.mDX11pSwapChain->Release();	
	}

	bool GfxWindow_DX11::ResourceInit()
	{
		RECT rc;
		GetClientRect( Get().mhWindow, &rc );
		mInstanceInfo.mvSize					= awVec2U16(awU16(rc.right-rc.left), awU16(rc.bottom-rc.top));
		mInstanceInfo.meBackbufferColorFormat	= awconst::keTexFormat_RGBA8;
		mInstanceInfo.meBackbufferDepthFormat	= awconst::keTexFormat_D24S8;

		DXGI_SWAP_CHAIN_DESC swapDesc;
		ZeroMemory( &swapDesc, sizeof( swapDesc ) );
		swapDesc.BufferCount						= 2;
		swapDesc.BufferDesc.Width					= mInstanceInfo.mvSize.x;
		swapDesc.BufferDesc.Height					= mInstanceInfo.mvSize.y;
		swapDesc.BufferDesc.Format					= EMgr::GfxRender.AWFormatToNative(mInstanceInfo.meBackbufferColorFormat);
		swapDesc.BufferDesc.RefreshRate.Numerator	= 60;
		swapDesc.BufferDesc.RefreshRate.Denominator	= 1;
		swapDesc.BufferUsage						= DXGI_USAGE_RENDER_TARGET_OUTPUT;
		swapDesc.OutputWindow						= mInstanceInfo.mhWindow;
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
					if( SUCCEEDED(pIDXGIFactory->CreateSwapChain( DX11pDevice, &swapDesc, &mInstanceInfo.mDX11pSwapChain)) )
					{
						mInstanceInfo.mrBackbufferColor = ERes::GfxRenderTarget_DX11::CreateFromBackuffer( mInstanceInfo.mDX11pSwapChain, mInstanceInfo.meBackbufferColorFormat, mInstanceInfo.mvSize );
						mInstanceInfo.mrBackbufferDepth = EExp::CreateGfxRenderTarget( mInstanceInfo.meBackbufferDepthFormat, mInstanceInfo.mvSize);
						mInstanceInfo.mrBackbufferView	= EExp::CreateGfxView( mInstanceInfo.mrBackbufferColor.GetResID(), mInstanceInfo.mrBackbufferDepth.GetResID() );
						return mInstanceInfo.mrBackbufferView.IsValid();
					}
		
		return false;;
	}

	void GfxWindow_DX11::Resize(const awVec2U16& _vSize)
	{
		mvPendingResize	 = _vSize;
	}

	//==================================================================================================
	//! @brief		Perform the resizing of the backbuffer
	//! @details	Called on frame start by ManagerRender to perform size changes requested earlier.
	//!				We should always be in a valid state for resizing when calling this 
	//!				(not in middle of a frame rendering)
	//--------------------------------------------------------------------------------------------------
	//! @return		
	//==================================================================================================
	void GfxWindow_DX11::PerformResize()
	{
		ERes::GfxWindowRef rWindowCur = EMgr::GfxRender.GetWindowCurrent();
		AWAssert(mInstanceInfo.mDX11pSwapChain);
		AWAssertMsg(rWindowCur != this, "This method should only be called in ManagerBase::FrameStart()");

		if( !mvPendingResize.IsNull() && mvPendingResize != mInstanceInfo.mvSize )
		{
			mInstanceInfo.mvSize			= mvPendingResize;			
			mInstanceInfo.mrBackbufferColor->ReleaseBackbuffer();
			mInstanceInfo.mDX11pSwapChain->ResizeBuffers(0, mvPendingResize.x, mvPendingResize.y, DXGI_FORMAT_UNKNOWN, 0);
			mInstanceInfo.mrBackbufferColor = ERes::GfxRenderTarget_DX11::CreateFromBackuffer( mInstanceInfo.mDX11pSwapChain, mInstanceInfo.meBackbufferColorFormat, mvPendingResize );
			mInstanceInfo.mrBackbufferDepth = EExp::CreateGfxRenderTarget( mInstanceInfo.meBackbufferDepthFormat, mvPendingResize);
			mInstanceInfo.mrBackbufferView	= EExp::CreateGfxView( mInstanceInfo.mrBackbufferColor.GetResID(), mInstanceInfo.mrBackbufferDepth.GetResID() );
		}		
		mvPendingResize.SetNull();
	}
}