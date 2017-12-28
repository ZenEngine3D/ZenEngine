#include "zcCore.h"

//SF DX12
namespace zcRes
{

GfxTarget2D_DX12::~GfxTarget2D_DX12()
{
}

//==================================================================================================
//! @brief		Create a RenderTarget from the backbuffer, skipping the export process
//==================================================================================================
GfxTarget2DRef GfxTarget2D_DX12::RuntimeCreate(const DirectXComRef<IDXGISwapChain>& _rSwapchain, zenConst::eTextureFormat _eTexFormat, zUInt _uBufferId)
{			
	GfxTarget2DRef					rRendertarget(nullptr);
	DirectXComRef<ID3D12Resource>	rDXRendertarget;
	HRESULT hr = _rSwapchain->GetBuffer((UINT)_uBufferId, IID_PPV_ARGS(&rDXRendertarget));
	if( SUCCEEDED(hr) )
	{		
		D3D12_RESOURCE_DESC RenderTargetDesc = rDXRendertarget->GetDesc();
		if( RenderTargetDesc.Dimension == D3D12_RESOURCE_DIMENSION_TEXTURE2D && RenderTargetDesc.Flags & D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET )
		{
			static zenMem::zAllocatorPool sMemPool("Pool GfxTarget2D", sizeof(GfxTarget2D), 128, 128);		
			rRendertarget						= zenNew(&sMemPool) GfxTarget2D();		
			rRendertarget.HAL()->meFormat		= _eTexFormat;
			rRendertarget.HAL()->mvDim			= zVec2U16((zU16)RenderTargetDesc.Width, (zU16)RenderTargetDesc.Height);
			rRendertarget.HAL()->mrResource		= rDXRendertarget;
			if( rRendertarget.HAL()->InitializeCommon(rRendertarget.HAL()->mrResource) )
			{
				RuntimeCreateFinalize( *rRendertarget.HAL(), zenConst::keResType_GfxTarget2D );
				return rRendertarget;
			}
		}
	}
	return nullptr;
}

//==================================================================================================
//==================================================================================================
bool GfxTarget2D_DX12::Initialize()
{
#if ZEN_RENDERER_DX12
	const DirectXComRef<ID3D12Resource> rSurface;
	GfxTarget2DRef	rRenderTarget	= (GfxTarget2D*)this;
	GfxTexture2DRef	rTexture		= rRenderTarget->GetTexture2D();
	return InitializeCommon(rTexture.HAL()->mResource.mrResource);

#else
	HRESULT hr(S_FALSE);
	ID3D11Texture2D*	pTexture(nullptr);	
	GfxTarget2DRef		rRenderTarget		= (GfxTarget2D*)this;
	GfxTexture2DRef		rTexture			= rRenderTarget->GetTexture2D();
	return InitializeCommon(*rTexture.HAL()->mpTextureBuffer);
#endif
	
}

//==================================================================================================
//! @brief		Creates the resource view on the render target
//==================================================================================================
bool GfxTarget2D_DX12::InitializeCommon(const DirectXComRef<ID3D12Resource>& _rTexture)
{
	if( zcMgr::GfxRender.IsDepth(meFormat) )
	{
		D3D12_DEPTH_STENCIL_VIEW_DESC DSVDesc={};
		DSVDesc.Format				= zcMgr::GfxRender.ZenFormatToDepthDSV(meFormat);
		DSVDesc.ViewDimension		= D3D12_DSV_DIMENSION_TEXTURE2D; //D3D12_DSV_DIMENSION_TEXTURE2DMS for multisample
		DSVDesc.Texture2D.MipSlice	= 0;
		DSVDesc.Flags				= D3D12_DSV_FLAG_NONE;//D3D12_DSV_FLAG_READ_ONLY_STENCIL; //D3D12_DSV_FLAG_READ_ONLY_DEPTH, ... @todo 2 features supports multiple depth config
		mTargetDepthView			= zcMgr::GfxRender.GetDescriptorDSV(1);
		zcMgr::GfxRender.GetDevice()->CreateDepthStencilView(_rTexture.Get(), &DSVDesc, mTargetDepthView.GetCpu() );				
		return true;
	}
	else
	{
		D3D12_RENDER_TARGET_VIEW_DESC RTVDesc={};
		RTVDesc.Format				= zcMgr::GfxRender.ZenFormatToNative(meFormat);
		RTVDesc.ViewDimension		= D3D12_RTV_DIMENSION_TEXTURE2D;
		RTVDesc.Texture2D			= {0, 0};
		mTargetColorView			= zcMgr::GfxRender.GetDescriptorRTV(1);
		zcMgr::GfxRender.GetDevice()->CreateRenderTargetView(_rTexture.Get(), &RTVDesc, mTargetColorView.GetCpu() );				

		HRESULT hr = zcMgr::GfxRender.GetDevice()->GetDeviceRemovedReason();
		return hr == S_OK;
	}
}

//==================================================================================================
//! @brief		Free up the reference to swapchain backbuffer
//! @details	Called GfxWindow_DX12::PerformResize to remove all reference to swapchain, before
//!				performing a resize on it
//--------------------------------------------------------------------------------------------------
//! @return		
//==================================================================================================
void GfxTarget2D_DX12::ReleaseBackbuffer()
{
#if !ZEN_RENDERER_DX12
	if( mpTargetColorView )
		mpTargetColorView->Release();
	mpTargetColorView = nullptr;
#endif
}

}
