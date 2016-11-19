#include "zcCore.h"

//SF DX12
namespace zcRes
{
	GfxWindowRef GfxWindowHAL_DX12::RuntimeCreate(HWND _WindowHandle)
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
						zcMgr::GfxRender.m_commandQueue.Get(),
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
													
		static zenMem::zAllocatorPool sMemPool("Pool GfxWindow", sizeof(GfxWindow), 32, 32);
		GfxWindowRef rResource						= zenNew(&sMemPool) GfxWindow();		
		bool bValid									= true;
		rSwapChain.As( &rResource.HAL()->mrDXSwapChain );
		rResource->muCurrentBackbuffer				= rResource.HAL()->mrDXSwapChain->GetCurrentBackBufferIndex();
		rResource.HAL()->mhWindow					= _WindowHandle;				
		rResource.HAL()->meBackbufferColorFormat	= eColorFormat;
		rResource.HAL()->mvSize						= zVec2U16(zU16(rc.right-rc.left), zU16(rc.bottom-rc.top));

		for(zUInt idx(0); idx<zenArrayCount(mrBackbufferColor) && bValid; ++idx)
		{
			rResource.HAL()->mrBackbufferColor[idx]	= GfxTarget2DHAL_DX12::RuntimeCreate(rSwapChain, eColorFormat, idx);							
			bValid									= rResource.HAL()->mrBackbufferColor[idx].IsValid();
		}

		if( bValid )
		{
			RuntimeCreateFinalize( *rResource.HAL(), zenConst::keResType_GfxWindow );
			return rResource;
		}

		return nullptr;
#if !DISABLE_DX12
		IDXGIDevice*				pDXGIDevice;
		IDXGIAdapter*				pDXGIAdapter;
		IDXGIFactory*				pDXGIFactory;
		IDXGISwapChain*				pDXSwapChain;

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
		ID3D12Device*   DX12pDevice					= zcMgr::GfxRender.DX12GetDevice();		
		
		HRESULT hr = DX12pDevice->QueryInterface(__uuidof(IDXGIDevice), (void **)&pDXGIDevice);
		if( SUCCEEDED(hr) )
		//if( SUCCEEDED(DX12pDevice->QueryInterface(__uuidof(IDXGIDevice), (void **)&pDXGIDevice)) )
		{
			hr = pDXGIDevice->GetParent(__uuidof(IDXGIAdapter), (void **)&pDXGIAdapter);
			if( SUCCEEDED(hr) )
			//if( SUCCEEDED(pDXGIDevice->GetParent(__uuidof(IDXGIAdapter), (void **)&pDXGIAdapter)) )
			{
				hr = pDXGIAdapter->GetParent(__uuidof(IDXGIFactory), (void **)&pDXGIFactory);
				if( SUCCEEDED(hr) )
				//if( SUCCEEDED(pDXGIAdapter->GetParent(__uuidof(IDXGIFactory), (void **)&pDXGIFactory)) )
				{
					hr = pDXGIFactory->CreateSwapChain( DX12pDevice, &SwapDesc, &pDXSwapChain);
					if( SUCCEEDED(hr) )
					//if( SUCCEEDED(pDXGIFactory->CreateSwapChain( DX12pDevice, &SwapDesc, &pDXSwapChain)) )
					{						
						static zenMem::zAllocatorPool sMemPool("Pool GfxWindow", sizeof(GfxWindow), 128, 128);
						GfxWindowRef rResource						= zenNew(&sMemPool) GfxWindow();		
						bool bValid									= true;						
						rResource.HAL()->mhWindow					= _WindowHandle;
						rResource.HAL()->mpDX12SwapChain			= pDXSwapChain;
						rResource.HAL()->meBackbufferColorFormat	= eColorFormat;
						rResource.HAL()->mvSize						= zVec2U16(zU16(rc.right-rc.left), zU16(rc.bottom-rc.top));
						for(zUInt idx(0); idx<zenArrayCount(mrBackbufferColor) && bValid; ++idx)
						{
							rResource.HAL()->mrBackbufferColor[idx]	= GfxTarget2DHAL_DX12::RuntimeCreate(*pDXSwapChain, eColorFormat, idx);							
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
#endif
	}

	GfxWindowHAL_DX12::~GfxWindowHAL_DX12()
	{
	#if !DISABLE_DX12
		if( mpDX12SwapChain )
			mpDX12SwapChain->Release();	
	#endif
	}

	//==================================================================================================
	//! @brief		Perform the resizing of the backbuffer
	//! @details	Called on frame start by ManagerRender to perform size changes requested earlier.
	//!				We should always be in a valid state for resizing when calling this 
	//!				(not in middle of a frame rendering)
	//--------------------------------------------------------------------------------------------------
	//! @return		
	//==================================================================================================
	bool GfxWindowHAL_DX12::PerformResize()
	{
		zenAssertMsg(zcGfx::grWindowRender.IsValid()==false || zcGfx::grWindowRender.Get() != dynamic_cast<GfxWindow*>(this), "This method should only be called in ManagerBase::FrameStart()");

		bool bResize = !mvPendingResize.IsZero() && mvPendingResize != mvSize;
	#if !DISABLE_DX12
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
				mrBackbufferColor[idx]	= GfxTarget2DHAL_DX12::RuntimeCreate(*mpDX12SwapChain, meBackbufferColorFormat, idx);				
				bValid					= mrBackbufferColor[idx].IsValid();
			}
		}
	#endif
		mvPendingResize.SetZero();
		return bResize;
	}

	void GfxWindowHAL_DX12::FrameBegin()
	{
		GfxWindow* pWindow				= reinterpret_cast<GfxWindow*>(this);
		UINT uIndexCurrent				= mrDXSwapChain->GetCurrentBackBufferIndex();
		pWindow->mrBackbufferCurrent	= mrBackbufferColor[uIndexCurrent];
	}

	void GfxWindowHAL_DX12::FrameEnd()
	{
	}
	
}