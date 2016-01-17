#include "zcCore.h"

namespace zcRes
{
	//////////////////////////////////////////////////////////////////////////
	// GfxSamplerProxy
	//////////////////////////////////////////////////////////////////////////
	GfxSamplerProxy_DX11::GfxSamplerProxy_DX11()
	: mpSamplerState(nullptr)
	{
	}
	
	GfxSamplerProxy_DX11::~GfxSamplerProxy_DX11()
	{
		if( mpSamplerState )
			mpSamplerState->Release();
		mpSamplerState = nullptr;
	}
	
	bool GfxSamplerProxy_DX11::Initialize(class GfxSampler& _Owner)
	{	
		const GfxSampler::ResDataRef& rResData = _Owner.GetResData();
		ZENAssert(rResData.IsValid());
		ZENDbgCode(mpOwner = &_Owner);
		HRESULT hr = zcMgr::GfxRender.DX11GetDevice()->CreateSamplerState( &rResData->mSamplerDesc, &mpSamplerState );
		return SUCCEEDED(hr);
	}
	
					
	//////////////////////////////////////////////////////////////////////////
	//GfxBlend_DX11
	//////////////////////////////////////////////////////////////////////////
	GfxStateBlendProxy_DX11::GfxStateBlendProxy_DX11()
	: mpBlendState(nullptr)
	{		
	}

	GfxStateBlendProxy_DX11::~GfxStateBlendProxy_DX11()
	{	
		if( mpBlendState )
			mpBlendState->Release();
		mpBlendState = nullptr;
	}

	bool GfxStateBlendProxy_DX11::Initialize(class GfxStateBlend& _Owner)
	{
		const GfxStateBlend::ResDataRef& rResData = _Owner.GetResData();
		ZENAssert(rResData.IsValid());
		ZENDbgCode(mpOwner = &_Owner);
		HRESULT hr = zcMgr::GfxRender.DX11GetDevice()->CreateBlendState( &rResData->mBlendDesc, &mpBlendState );
		//! @note : Global write mask / Blend factor : Unused functionality for now (per render target for now)
		muSampleMask = 0xFFFFFFFF;				
		for(zUInt i(0); i<ZENArrayCount(mafBlendFactor); ++i)
			mafBlendFactor[i] = 0.0f;

		return SUCCEEDED(hr);
	}

	//////////////////////////////////////////////////////////////////////////
	//GfxDepthStencil_DX11
	//////////////////////////////////////////////////////////////////////////
	GfxStateDepthStencilProxy_DX11::GfxStateDepthStencilProxy_DX11()
	: mpDepthStencilState(nullptr)
	{		
	}

	GfxStateDepthStencilProxy_DX11::~GfxStateDepthStencilProxy_DX11()
	{	
		if( mpDepthStencilState )
			mpDepthStencilState->Release();
		mpDepthStencilState = nullptr;
	}

	bool GfxStateDepthStencilProxy_DX11::Initialize(class GfxStateDepthStencil& _Owner)
	{
		const GfxStateDepthStencil::ResDataRef& rResData = _Owner.GetResData();
		ZENAssert(rResData.IsValid());
		ZENDbgCode(mpOwner = &_Owner);
		HRESULT hr = zcMgr::GfxRender.DX11GetDevice()->CreateDepthStencilState( &rResData->mDepthStencilDesc, &mpDepthStencilState );
		muStencilValue = 0;
		return SUCCEEDED(hr);
	}

	//////////////////////////////////////////////////////////////////////////
	//GfxRasterizer_DX11
	//////////////////////////////////////////////////////////////////////////
	GfxStateRasterizerProxy_DX11::GfxStateRasterizerProxy_DX11()
	: mpRasterizerState(nullptr)
	{		
	}

	GfxStateRasterizerProxy_DX11::~GfxStateRasterizerProxy_DX11()
	{	
		if( mpRasterizerState )
			mpRasterizerState->Release();
		mpRasterizerState = nullptr;
	}

	bool GfxStateRasterizerProxy_DX11::Initialize(class GfxStateRasterizer& _Owner)
	{
		const GfxStateRasterizer::ResDataRef& rResData = _Owner.GetResData();
		ZENAssert(rResData.IsValid());
		ZENDbgCode(mpOwner = &_Owner);
		HRESULT hr			= zcMgr::GfxRender.DX11GetDevice()->CreateRasterizerState( &rResData->mRasterizerDesc, &mpRasterizerState );
		mbScreenScissorOn	= rResData->mRasterizerDesc.ScissorEnable > 0;
		return SUCCEEDED(hr);
	}
}
