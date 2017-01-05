#include "zcCore.h"

//SF DX12
namespace zcRes
{

GfxTarget2DHAL_DX12::~GfxTarget2DHAL_DX12()
{
	mTargetColorView.Free();
	mTargetDepthView.Free();
}

//==================================================================================================
//! @brief		Create a RenderTarget from the backbuffer, skipping the export process
//==================================================================================================
GfxTarget2DRef GfxTarget2DHAL_DX12::RuntimeCreate(const DirectXComRef<IDXGISwapChain>& _rSwapchain, zenConst::eTextureFormat _eTexFormat, zUInt _uBufferId)
{			
	GfxTarget2DRef					rResource(nullptr);
	DirectXComRef<ID3D12Resource>	rRendertarget;
	HRESULT hr = _rSwapchain->GetBuffer((UINT)_uBufferId, IID_PPV_ARGS(&rRendertarget));
	if( SUCCEEDED(hr) )
	{		
		D3D12_RESOURCE_DESC RenderTargetDesc = rRendertarget->GetDesc();
		if( RenderTargetDesc.Dimension == D3D12_RESOURCE_DIMENSION_TEXTURE2D && RenderTargetDesc.Flags & D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET )
		{
			static zenMem::zAllocatorPool sMemPool("Pool GfxTarget2D", sizeof(GfxTarget2D), 128, 128);		
			rResource							= zenNew(&sMemPool) GfxTarget2D();		
			rResource.HAL()->meFormat			= _eTexFormat;
			rResource.HAL()->mvDim				= zVec2U16((zU16)RenderTargetDesc.Width, (zU16)RenderTargetDesc.Height);
			rResource.HAL()->mrTextureResource	= rRendertarget;
			if( rResource.HAL()->InitializeCommon(rResource.HAL()->mrTextureResource) )
			{
				RuntimeCreateFinalize( *rResource.HAL(), zenConst::keResType_GfxTarget2D );
				return rResource;
			}
		}
	}
	return nullptr;
}

//==================================================================================================
//==================================================================================================
bool GfxTarget2DHAL_DX12::Initialize()
{
#if ZEN_RENDERER_DX12
	const DirectXComRef<ID3D12Resource> rSurface;
	GfxTarget2DRef	rRenderTarget	= (GfxTarget2D*)this;
	GfxTexture2dRef	rTexture		= rRenderTarget->GetTexture2D();
	return InitializeCommon(rTexture.HAL()->mrTextureResource);

#else
	HRESULT hr(S_FALSE);
	ID3D11Texture2D*	pTexture(nullptr);	
	GfxTarget2DRef		rRenderTarget		= (GfxTarget2D*)this;
	GfxTexture2dRef		rTexture			= rRenderTarget->GetTexture2D();
	return InitializeCommon(*rTexture.HAL()->mpTextureBuffer);
#endif
	
}

//==================================================================================================
//! @brief		Creates the resource view on the render target
//==================================================================================================
bool GfxTarget2DHAL_DX12::InitializeCommon(const DirectXComRef<ID3D12Resource>& _rTexture)
{
	if( zcMgr::GfxRender.IsDepth(meFormat) )
	{
		D3D12_DEPTH_STENCIL_VIEW_DESC DSVDesc={};
		DSVDesc.Format				= zcMgr::GfxRender.ZenFormatToDepthDSV(meFormat);
		DSVDesc.ViewDimension		= D3D12_DSV_DIMENSION_TEXTURE2D; //D3D12_DSV_DIMENSION_TEXTURE2DMS for multisample
		DSVDesc.Texture2D.MipSlice	= 0;
		DSVDesc.Flags				= D3D12_DSV_FLAG_NONE;//D3D12_DSV_FLAG_READ_ONLY_STENCIL; //D3D12_DSV_FLAG_READ_ONLY_DEPTH, ... @todo 2 features supports multiple depth config
		mTargetDepthView			= zcGfx::DescriptorDSV::Allocate();
		zcMgr::GfxRender.GetDevice()->CreateDepthStencilView(_rTexture.Get(), &DSVDesc, mTargetDepthView.GetHandle() );				
		return true;
	}
	else
	{
		D3D12_RENDER_TARGET_VIEW_DESC RTVDesc={};
		RTVDesc.Format				= zcMgr::GfxRender.ZenFormatToNative(meFormat);
		RTVDesc.ViewDimension		= D3D12_RTV_DIMENSION_TEXTURE2D;
		RTVDesc.Texture2D			= {0, 0};
		mTargetColorView			= zcGfx::DescriptorRTV::Allocate();
		zcMgr::GfxRender.GetDevice()->CreateRenderTargetView(_rTexture.Get(), &RTVDesc, mTargetColorView.GetHandle() );				

		HRESULT hr = zcMgr::GfxRender.GetDevice()->GetDeviceRemovedReason();
		return true;
	}
	return false;
}

//==================================================================================================
//! @brief		Free up the reference to swapchain backbuffer
//! @details	Called GfxWindow_DX12::PerformResize to remove all reference to swapchain, before
//!				performing a resize on it
//--------------------------------------------------------------------------------------------------
//! @return		
//==================================================================================================
void GfxTarget2DHAL_DX12::ReleaseBackbuffer()
{
#if !ZEN_RENDERER_DX12
	if( mpTargetColorView )
		mpTargetColorView->Release();
	mpTargetColorView = nullptr;
#endif
}

void GfxTarget2DHAL_DX12::Clear(const zVec4F& _vRGBA)
{
	zenAssertMsg( !IsDepth(), "Trying to clear a depth rendertarget as color.");		
	zcMgr::GfxRender.m_commandList->ClearRenderTargetView(mTargetColorView.GetHandle(), _vRGBA.xyzw, 0, nullptr);
}

void GfxTarget2DHAL_DX12::Clear(float _fDepth, zU8 _uStencil, bool _bClearDepth, bool _bClearStencil)
{
	zenAssertMsg( IsDepth(), "Trying to clear a color rendertarget as depth.");
	D3D12_CLEAR_FLAGS ClearFlags  = _bClearDepth	? D3D12_CLEAR_FLAG_DEPTH	: (D3D12_CLEAR_FLAGS)0;
	//ClearFlags		|= _bClearStencil	? D3D12_CLEAR_FLAG_STENCIL	: 0; //!todo 2 support stencil
//	zcMgr::GfxRender.m_commandList->ClearDepthStencilView(mTargetDepthView.GetHandle(), ClearFlags, _fDepth, 0, 0, nullptr );
}

}
