#include "libZenCore.h"

namespace EMgr { EGfx::ManagerState GfxState; }

namespace EGfx
{

ManagerState_DX11::ManagerState_DX11()
: mrView(NULL)
, mpStateBlend(NULL)
, mpStateDepthStencil(NULL)
, mbUpdatedView(false)
, mbUpdatedBlend(false)
, mbUpdatedDepthStencil(false)
{

}

bool ManagerState_DX11::Load()
{	
	// Init conversion table for Blending WriteMask
	for(zU8 uBlendMask=0; uBlendMask<16; ++uBlendMask)
	{
		mBlendWriteMaskToDX11[uBlendMask]	= 0;
		mBlendWriteMaskToDX11[uBlendMask]	|= (uBlendMask&ConfigBlend::keWrite_R) ? D3D11_COLOR_WRITE_ENABLE_RED : 0;
		mBlendWriteMaskToDX11[uBlendMask]	|= (uBlendMask&ConfigBlend::keWrite_G) ? D3D11_COLOR_WRITE_ENABLE_GREEN : 0;
		mBlendWriteMaskToDX11[uBlendMask]	|= (uBlendMask&ConfigBlend::keWrite_B) ? D3D11_COLOR_WRITE_ENABLE_BLUE : 0;
		mBlendWriteMaskToDX11[uBlendMask]	|= (uBlendMask&ConfigBlend::keWrite_A) ? D3D11_COLOR_WRITE_ENABLE_ALPHA : 0;		
	}
	// Init conversion table for Blending channel operations
	memset(mBlendToDX11, 0xFF, sizeof(mBlendToDX11) );
	mBlendToDX11[ConfigBlend::keBlend_None].Set		(D3D11_BLEND_ONE,		D3D11_BLEND_ZERO,			D3D11_BLEND_OP_ADD);
	mBlendToDX11[ConfigBlend::keAlphaMask].Set		(D3D11_BLEND_ONE,		D3D11_BLEND_ZERO,			D3D11_BLEND_OP_ADD);
	mBlendToDX11[ConfigBlend::keBlend_Alpha].Set	(D3D11_BLEND_SRC_ALPHA,	D3D11_BLEND_INV_SRC_ALPHA,	D3D11_BLEND_OP_ADD);
	mBlendToDX11[ConfigBlend::keBlend_Add].Set		(D3D11_BLEND_SRC_ALPHA,	D3D11_BLEND_ONE,			D3D11_BLEND_OP_ADD);
	mBlendToDX11[ConfigBlend::keBlend_Sub].Set		(D3D11_BLEND_SRC_ALPHA,	D3D11_BLEND_ONE,			D3D11_BLEND_OP_SUBTRACT);
	mBlendToDX11[ConfigBlend::keBlend_Modulate].Set	(D3D11_BLEND_ZERO,		D3D11_BLEND_SRC_COLOR,		D3D11_BLEND_OP_ADD);
	mBlendToDX11[ConfigBlend::keBlend_Min].Set		(D3D11_BLEND_ONE,		D3D11_BLEND_ONE,			D3D11_BLEND_OP_MIN);
	mBlendToDX11[ConfigBlend::keBlend_Max].Set		(D3D11_BLEND_ONE,		D3D11_BLEND_ONE,			D3D11_BLEND_OP_MAX);
	
	// Init conversion tablt for Depth test operation
	mDepthTestToDX11[ConfigDepthStencil::keDepth_Always]	= D3D11_COMPARISON_ALWAYS;
	mDepthTestToDX11[ConfigDepthStencil::keDepth_Equal]		= D3D11_COMPARISON_EQUAL;
	mDepthTestToDX11[ConfigDepthStencil::keDepth_NEqual]	= D3D11_COMPARISON_NOT_EQUAL;
	mDepthTestToDX11[ConfigDepthStencil::keDepth_Less]		= D3D11_COMPARISON_LESS;
	mDepthTestToDX11[ConfigDepthStencil::keDepth_LessE]		= D3D11_COMPARISON_LESS_EQUAL;
	mDepthTestToDX11[ConfigDepthStencil::keDepth_Greater]	= D3D11_COMPARISON_GREATER;
	mDepthTestToDX11[ConfigDepthStencil::keDepth_GreaterE]	= D3D11_COMPARISON_GREATER_EQUAL;
	
	mbUsingMRTs = false;
	return true;
}

bool ManagerState_DX11::Unload()
{
	return true;
}

void ManagerState_DX11::SetBlend(const ConfigBlend& _Blend, zUInt _uRenderTarget)
{
	ZENAssert(_uRenderTarget<kuMaxRenderTarget);
	if( mConfigBlend[_uRenderTarget] != _Blend )
	{
		mbUpdatedBlend					= true;
		mConfigBlend[_uRenderTarget]	= _Blend;
	}		
}
void ManagerState_DX11::SetView(ERes::GfxViewRef _rView)
{
	if( mrView != _rView )
	{
		mrView			= _rView;
		mbUpdatedView	= true;
	}
}
void ManagerState_DX11::PipelineReset()
{
	mbUpdatedView			= true;
	mbUpdatedDepthStencil	= true;
	mbUpdatedBlend			= true;
}

void ManagerState_DX11::PipelineUpdate()
{
	ID3D11Device* pDevice			= EMgr::GfxRender.DX11GetDevice();
	ID3D11DeviceContext* pContext	= EMgr::GfxRender.DX11GetDeviceContext();

	if( mbUpdatedView )
	{
		// Make sure all RenderTarget are unbound as texture, before setting them back as target
		EMgr::GfxRender.UnbindTextures();
		
		ID3D11RenderTargetView * apColorViews[kuMaxRenderTarget];
		ID3D11DepthStencilView* pDepthView(NULL);
		D3D11_VIEWPORT vp;
		
		zenMem::Set(apColorViews, 0, sizeof(apColorViews));
		zenMem::Set(&vp, 0, sizeof(vp));		
		if( mrView.IsValid() )
		{
			// Setup rendertarget
			zUInt targetCount = mrView->Get().marTargetColor.Count();
			for( zUInt targetIdx(0); targetIdx<kuMaxRenderTarget; ++targetIdx)
			{
				ERes::GfxRenderTargetRef rTarget	= (targetIdx<targetCount) ? mrView->Get().marTargetColor[targetIdx] : NULL;
				apColorViews[targetIdx]				= rTarget.IsValid() ? rTarget->Get().mpTargetColorView : NULL;
			}
			pDepthView = mrView->Get().mrTargetDepth.IsValid() ? mrView->Get().mrTargetDepth->Get().mpTargetDepthView : NULL;
			
			// Setup the viewport		
			vp.Width	= (FLOAT)mrView->Get().mvDim.x;
			vp.Height	= (FLOAT)mrView->Get().mvDim.y;
			vp.MinDepth = 0.0f;
			vp.MaxDepth = 1.0f;
			vp.TopLeftX = (FLOAT)mrView->Get().mvOrigin.x;
			vp.TopLeftY = (FLOAT)mrView->Get().mvOrigin.y;
		}
		
		EMgr::GfxRender.DX11GetDeviceContext()->OMSetRenderTargets( kuMaxRenderTarget, apColorViews, pDepthView );
		EMgr::GfxRender.DX11GetDeviceContext()->RSSetViewports( 1, &vp );
	}
	

	//---------------------------------------------------------------
	// Update Blending State of RenderTargets
	//---------------------------------------------------------------
	/*if( muUpdateBlend )
	{
		D3D11_BLEND_DESC BlendState;
		zenMem::Set(&BlendState, 0, sizeof(D3D11_BLEND_DESC));
		for(int i=0; i<kuMaxRenderTarget; ++i)
		{
			ConfigBlend& Config								= mConfigBlend[i];
			D3D11_RENDER_TARGET_BLEND_DESC&	RenderTarget	= BlendState.RenderTarget[i];
			ZENAssertMsg( Config.muWriteMask < 16, "Invalid WriteMask specified");
			ZENAssertMsg( Config.meBlendRGB < ConfigBlend::keBlend__Count && Config.meBlendAlpha < ConfigBlend::keBlend__Count, "Invalid Blending mode specified");
			ZENAssertMsg( mBlendToDX11[Config.meBlendRGB].Op != 0xFF && mBlendToDX11[Config.meBlendAlpha].Op != 0xFF, "Blending mode not supported on this platform renderer." );
			RenderTarget.BlendEnable			= Config.meBlendRGB != ConfigBlend::keBlend_None && Config.meBlendAlpha != ConfigBlend::keBlend_None;
			RenderTarget.RenderTargetWriteMask	= mBlendWriteMaskToDX11[Config.muWriteMask];
			RenderTarget.SrcBlend				= mBlendToDX11[Config.meBlendRGB].Src;
			RenderTarget.DestBlend				= mBlendToDX11[Config.meBlendRGB].Dst;
			RenderTarget.BlendOp				= mBlendToDX11[Config.meBlendRGB].Op;
			RenderTarget.SrcBlendAlpha			= mBlendToDX11[Config.meBlendAlpha].Src;
			RenderTarget.DestBlendAlpha			= mBlendToDX11[Config.meBlendAlpha].Dst;
			RenderTarget.BlendOpAlpha			= mBlendToDX11[Config.meBlendAlpha].Op;
		}

		UINT sampleMask(0xffffffff);									//! @note : Global write mask: Unused functionality for now (per render target for now)
		float blendFactor[4] = { 0.0f, 0.0f, 0.0f, 0.0f };				//! @note : Global blending factor: Unused functionality for now  
		if( mpStateBlend )
			mpStateBlend->Release();
		pDevice->CreateBlendState(&BlendState, &mpStateBlend);			//! @todo : Create Shader cache manager to re-used created states instead
		pContext->OMSetBlendState(mpStateBlend, blendFactor, sampleMask);
	}
	*/
	//---------------------------------------------------------------
	// Update Depth/Stencil pipeline state
	//---------------------------------------------------------------
	if( mbUpdatedDepthStencil )
	{
		D3D11_DEPTH_STENCIL_DESC DepthStencil;
		zenMem::Set(&DepthStencil, 0, sizeof(D3D11_DEPTH_STENCIL_DESC));
		// Depth test parameters
		DepthStencil.DepthEnable	= mConfigDepthStencil.meDepthTest != ConfigDepthStencil::keDepth_Always;
		DepthStencil.DepthWriteMask	= mConfigDepthStencil.mbDepthWrite ? D3D11_DEPTH_WRITE_MASK_ALL : D3D11_DEPTH_WRITE_MASK_ZERO;
		DepthStencil.DepthFunc		= mDepthTestToDX11[mConfigDepthStencil.meDepthTest];

		//! @todo : Implement stencil support
		DepthStencil.StencilEnable					= false;
		DepthStencil.StencilReadMask				= 0xFF;
		DepthStencil.StencilWriteMask				= 0xFF;
		DepthStencil.FrontFace.StencilFailOp		= D3D11_STENCIL_OP_KEEP;
		DepthStencil.FrontFace.StencilDepthFailOp	= D3D11_STENCIL_OP_INCR;
		DepthStencil.FrontFace.StencilPassOp		= D3D11_STENCIL_OP_KEEP;
		DepthStencil.FrontFace.StencilFunc			= D3D11_COMPARISON_ALWAYS;
		DepthStencil.BackFace.StencilFailOp			= D3D11_STENCIL_OP_KEEP;
		DepthStencil.BackFace.StencilDepthFailOp	= D3D11_STENCIL_OP_DECR;
		DepthStencil.BackFace.StencilPassOp			= D3D11_STENCIL_OP_KEEP;
		DepthStencil.BackFace.StencilFunc			= D3D11_COMPARISON_ALWAYS;

		// Create depth stencil state
		if( mpStateDepthStencil )
			mpStateDepthStencil->Release();
		pDevice->CreateDepthStencilState(&DepthStencil, &mpStateDepthStencil);
		pContext->OMSetDepthStencilState(mpStateDepthStencil, mConfigDepthStencil.muStencilRef);
	}
	
	mbUpdatedView			= false;
	mbUpdatedDepthStencil	= false;
	mbUpdatedBlend			= false;
}


}
