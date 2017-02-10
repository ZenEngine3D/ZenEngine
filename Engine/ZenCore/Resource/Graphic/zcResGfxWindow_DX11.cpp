#include "zcCore.h"

namespace zcRes
{
	GfxWindowRef GfxWindow_DX11::RuntimeCreate(HWND _WindowHandle)
	{
		RECT						rc;
		IDXGIDevice*				pDXGIDevice;
		IDXGIAdapter*				pDXGIAdapter;
		IDXGIFactory*				pDXGIFactory;
		IDXGISwapChain*				pDXSwapChain;
		DXGI_SWAP_CHAIN_DESC		SwapDesc;
		zenConst::eTextureFormat	eColorFormat	= zenConst::keTexFormat_RGBA8; //! @todo clean feature expose desired format in ResData
		
		GetClientRect( _WindowHandle, &rc );		
		ZeroMemory( &SwapDesc, sizeof( SwapDesc ) );		
		SwapDesc.BufferCount						= 2;
		SwapDesc.BufferDesc.Width					= rc.right-rc.left;
		SwapDesc.BufferDesc.Height					= rc.bottom-rc.top;
		SwapDesc.BufferDesc.Format					= zcMgr::GfxRender.ZenFormatToNative(eColorFormat);
		SwapDesc.BufferDesc.RefreshRate.Numerator	= 60;
		SwapDesc.BufferDesc.RefreshRate.Denominator	= 1;
		SwapDesc.BufferUsage						= DXGI_USAGE_RENDER_TARGET_OUTPUT;
		SwapDesc.BufferCount						= 2;
		SwapDesc.OutputWindow						= _WindowHandle;
		SwapDesc.SampleDesc.Count					= 1;
		SwapDesc.SampleDesc.Quality					= 0;
		SwapDesc.Windowed							= TRUE;
		SwapDesc.SwapEffect							= DXGI_SWAP_EFFECT_DISCARD;
		ID3D11Device*   DX11pDevice					= zcMgr::GfxRender.GetDevice();		
		
		if( SUCCEEDED(DX11pDevice->QueryInterface(__uuidof(IDXGIDevice), (void **)&pDXGIDevice)) )
		{
			if( SUCCEEDED(pDXGIDevice->GetParent(__uuidof(IDXGIAdapter), (void **)&pDXGIAdapter)) )
			{
				if( SUCCEEDED(pDXGIAdapter->GetParent(__uuidof(IDXGIFactory), (void **)&pDXGIFactory)) )
				{
					if( SUCCEEDED(pDXGIFactory->CreateSwapChain( DX11pDevice, &SwapDesc, &pDXSwapChain)) )
					{						
						static zenMem::zAllocatorPool sMemPool("Pool GfxWindow", sizeof(GfxWindow), 32, 32);
						GfxWindowRef rResource						= zenNew(&sMemPool) GfxWindow();		
						bool bValid									= true;						
						rResource.HAL()->mhWindow					= _WindowHandle;
						rResource.HAL()->mpDX11SwapChain			= pDXSwapChain;
						rResource.HAL()->meBackbufferColorFormat	= eColorFormat;
						rResource.HAL()->mvSize						= zVec2U16(zU16(rc.right-rc.left), zU16(rc.bottom-rc.top));
						for(zUInt idx(0); idx<zenArrayCount(mrBackbufferColor) && bValid; ++idx)
						{
							rResource.HAL()->mrBackbufferColor[idx]	= GfxTarget2D_DX11::RuntimeCreate(*pDXSwapChain, eColorFormat, idx);							
							bValid									= rResource.HAL()->mrBackbufferColor[idx].IsValid();
						}

						if( bValid )
						{
							RuntimeCreateFinalize( *rResource.HAL(), zenConst::keResType_GfxWindow );
							return rResource;
						}

					}
				}
			}
		}		
		return nullptr;		
	}

	GfxWindow_DX11::~GfxWindow_DX11()
	{
		if( mpDX11SwapChain )
			mpDX11SwapChain->Release();	
	}

	//==================================================================================================
	//! @brief		Perform the resizing of the backbuffer
	//! @details	Called on frame start by ManagerRender to perform size changes requested earlier.
	//!				We should always be in a valid state for resizing when calling this 
	//!				(not in middle of a frame rendering)
	//--------------------------------------------------------------------------------------------------
	//! @return		
	//==================================================================================================
	bool GfxWindow_DX11::PerformResize()
	{
		zenAssert(mpDX11SwapChain);
		zenAssertMsg(zcGfx::grWindowRender.IsValid()==false || zcGfx::grWindowRender.Get() != dynamic_cast<GfxWindow*>(this), "This method should only be called in ManagerBase::FrameStart()");

		bool bResize = !mvPendingResize.IsZero() && mvPendingResize != mvSize;
		if( bResize  )
		{				
			bool bValid(true);
			for(zUInt idx(0); idx<zenArrayCount(mrBackbufferColor); ++idx)
			{
				const GfxTarget2DRef& rRenderTarget = mrBackbufferColor[idx];
				rRenderTarget.HAL()->ReleaseBackbuffer();
			}
			mpDX11SwapChain->ResizeBuffers(0, mvPendingResize.x, mvPendingResize.y, DXGI_FORMAT_UNKNOWN, 0);
			
			for(zUInt idx(0); idx<zenArrayCount(mrBackbufferColor) && bValid; ++idx)
			{
				mrBackbufferColor[idx]	= GfxTarget2D_DX11::RuntimeCreate(*mpDX11SwapChain, meBackbufferColorFormat, idx);				
				bValid					= mrBackbufferColor[idx].IsValid();
			}
		}
		
		mvPendingResize.SetZero();
		return bResize;
	}

	void GfxWindow_DX11::FrameBegin()
	{
		GfxWindow* pWindow				= reinterpret_cast<GfxWindow*>(this);
		pWindow->mrBackbufferCurrent	= mrBackbufferColor[pWindow->GetFrameCount()%zenArrayCount(mrBackbufferColor)];
	}

	void GfxWindow_DX11::FrameEnd()
	{
	}
}