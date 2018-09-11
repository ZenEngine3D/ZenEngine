#include "zcCore.h"

//SF DX12
namespace zcRes
{
	GfxWindowRef GfxWindow_DX12::RuntimeCreate(HWND _WindowHandle)
	{
		RECT						rc;		
		zenConst::eTextureFormat	eColorFormat = zenConst::keTexFormat_RGBA8; //! @todo clean feature expose desired format in ResData		
		GetClientRect( _WindowHandle, &rc );		

		DXGI_SWAP_CHAIN_DESC1 SwapchainDesc = {};;
		SwapchainDesc.Width					= rc.right-rc.left;
		SwapchainDesc.Height				= rc.bottom-rc.top;
		SwapchainDesc.Format				= zcMgr::GfxRender.ZenFormatToNative(eColorFormat);
		SwapchainDesc.Stereo				= FALSE;
		SwapchainDesc.SampleDesc.Count		= 1;
		SwapchainDesc.SampleDesc.Quality	= 0;
		SwapchainDesc.BufferUsage			= DXGI_USAGE_RENDER_TARGET_OUTPUT;
		SwapchainDesc.BufferCount			= zenArrayCount(mrBackbufferColor);
		SwapchainDesc.Scaling				= DXGI_SCALING_STRETCH;
		SwapchainDesc.SwapEffect			= DXGI_SWAP_EFFECT_FLIP_DISCARD;
		SwapchainDesc.AlphaMode				= DXGI_ALPHA_MODE_UNSPECIFIED;
		SwapchainDesc.Flags					= 0;
		
		DirectXComRef<IDXGISwapChain1>	rSwapChain;

		HRESULT hr = zcMgr::GfxRender.GetFactory()->CreateSwapChainForHwnd(
						zcMgr::GfxRender.mrCommandQueue.Get(),
						_WindowHandle,
						&SwapchainDesc,
						nullptr,
						nullptr,
						&rSwapChain );
		if( FAILED(hr) )
			return FALSE;

		// This sample does not support fullscreen transitions.
		hr = zcMgr::GfxRender.GetFactory()->MakeWindowAssociation(_WindowHandle, DXGI_MWA_NO_ALT_ENTER);
		if( FAILED(hr) )
			return FALSE;
													
		//static zenMem::zAllocatorPool sMemPool("Pool GfxWindow", sizeof(GfxWindow), 32, 32);
		GfxWindowRef rResource						= zenNewPool GfxWindow();		
		bool bValid									= true;
		rSwapChain.As( &rResource.HAL()->mrDXSwapChain );
		rResource.HAL()->mhWindow					= _WindowHandle;				
		rResource.HAL()->meBackbufferColorFormat	= eColorFormat;
		rResource.HAL()->mvSize						= zVec2U16(zU16(rc.right-rc.left), zU16(rc.bottom-rc.top));

		for(zUInt idx(0); idx<zenArrayCount(mrBackbufferColor) && bValid; ++idx)
		{
			rResource.HAL()->mrBackbufferColor[idx]	= GfxTarget2D_DX12::RuntimeCreate(rSwapChain, eColorFormat, idx);							
			bValid									= rResource.HAL()->mrBackbufferColor[idx].IsValid();
		}

		if( bValid )
		{
			RuntimeCreateFinalize( *rResource.HAL(), zenConst::keResType_GfxWindow );
			return rResource;
		}

		return nullptr;
	}

	GfxWindow_DX12::~GfxWindow_DX12()
	{
	}

	//==================================================================================================
	//! @brief		Perform the resizing of the backbuffer
	//! @details	Called on frame start by ManagerRender to perform size changes requested earlier.
	//!				We should always be in a valid state for resizing when calling this 
	//!				(not in middle of a frame rendering)
	//--------------------------------------------------------------------------------------------------
	//! @return		
	//==================================================================================================
	bool GfxWindow_DX12::PerformResize()
	{
		zenAssertMsg(zcGfx::grWindowRender.IsValid()==false || zcGfx::grWindowRender.Get() != dynamic_cast<GfxWindow*>(this), "This method should only be called in ManagerBase::FrameStart()");

		bool bResize = !mvPendingResize.IsZero() && mvPendingResize != mvSize;
	#if !ZEN_RENDERER_DX12
		zenAssert(mpDX12SwapChain);
		if( bResize  )
		{				
			bool bValid(true);
			for(zUInt idx(0); idx<zenArrayCount(mrBackbufferColor); ++idx)
			{
				const GfxTarget2DRef& rRenderTarget = mrBackbufferColor[idx];
				rRenderTarget.HAL()->ReleaseBackbuffer();
			}
			mpDX12SwapChain->ResizeBuffers(0, mvPendingResize.x, mvPendingResize.y, DXGI_FORMAT_UNKNOWN, 0);
			
			for(zUInt idx(0); idx<zenArrayCount(mrBackbufferColor) && bValid; ++idx)
			{
				mrBackbufferColor[idx]	= GfxTarget2D_DX12::RuntimeCreate(*mpDX12SwapChain, meBackbufferColorFormat, idx);				
				bValid					= mrBackbufferColor[idx].IsValid();
			}
		}
	#endif
		mvPendingResize.SetZero();
		return bResize;
	}

	void GfxWindow_DX12::FrameBegin()
	{
		GfxWindow* pWindow				= reinterpret_cast<GfxWindow*>(this);
		UINT uIndexCurrent				= mrDXSwapChain->GetCurrentBackBufferIndex();
		pWindow->mrBackbufferCurrent	= mrBackbufferColor[uIndexCurrent];
	}

	void GfxWindow_DX12::FrameEnd()
	{
	}
	
}