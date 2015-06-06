#include "zcCore.h"

namespace zcRes
{
	//////////////////////////////////////////////////////////////////////////
	// GfxSamplerProxy
	//////////////////////////////////////////////////////////////////////////
	GfxSamplerProxy_DX11::GfxSamplerProxy_DX11()
	: mpSamplerState(NULL)
	{
	}
	
	GfxSamplerProxy_DX11::~GfxSamplerProxy_DX11()
	{
		if( mpSamplerState )
			mpSamplerState->Release();
		mpSamplerState = NULL;
	}
	
	bool GfxSamplerProxy_DX11::Initialize(class GfxSampler& _Owner)
	{	
		const GfxSampler::ResDataRef& rResData = _Owner.GetResData();
		ZENAssert(rResData.IsValid());
		ZENDbgCode(mpOwner = &_Owner);
		HRESULT hr = EMgr::GfxRender.DX11GetDevice()->CreateSamplerState( &rResData->mSamplerDesc, &mpSamplerState );
		return SUCCEEDED(hr);
	}
	
					
	//////////////////////////////////////////////////////////////////////////
	//GfxBlend_DX11
	//////////////////////////////////////////////////////////////////////////
	GfxStateBlendProxy_DX11::GfxStateBlendProxy_DX11()
	: mpBlendState(NULL)
	{		
	}

	GfxStateBlendProxy_DX11::~GfxStateBlendProxy_DX11()
	{	
		if( mpBlendState )
			mpBlendState->Release();
		mpBlendState = NULL;
	}

	bool GfxStateBlendProxy_DX11::Initialize(class GfxStateBlend& _Owner)
	{
		const GfxStateBlend::ResDataRef& rResData = _Owner.GetResData();
		ZENAssert(rResData.IsValid());
		ZENDbgCode(mpOwner = &_Owner);
		HRESULT hr = EMgr::GfxRender.DX11GetDevice()->CreateBlendState( &rResData->mBlendDesc, &mpBlendState );
		return SUCCEEDED(hr);
	}

	//////////////////////////////////////////////////////////////////////////
	//GfxDepthStencil_DX11
	//////////////////////////////////////////////////////////////////////////
	GfxStateDepthStencilProxy_DX11::GfxStateDepthStencilProxy_DX11()
	: mpDepthStencilState(NULL)
	{		
	}

	GfxStateDepthStencilProxy_DX11::~GfxStateDepthStencilProxy_DX11()
	{	
		if( mpDepthStencilState )
			mpDepthStencilState->Release();
		mpDepthStencilState = NULL;
	}

	bool GfxStateDepthStencilProxy_DX11::Initialize(class GfxStateDepthStencil& _Owner)
	{
		const GfxStateDepthStencil::ResDataRef& rResData = _Owner.GetResData();
		ZENAssert(rResData.IsValid());
		ZENDbgCode(mpOwner = &_Owner);
		HRESULT hr = EMgr::GfxRender.DX11GetDevice()->CreateDepthStencilState( &rResData->mDepthStencilDesc, &mpDepthStencilState );
		return SUCCEEDED(hr);
	}

	//////////////////////////////////////////////////////////////////////////
	//GfxRasterizer_DX11
	//////////////////////////////////////////////////////////////////////////
	GfxStateRasterizerProxy_DX11::GfxStateRasterizerProxy_DX11()
	: mpRasterizerState(NULL)
	{		
	}

	GfxStateRasterizerProxy_DX11::~GfxStateRasterizerProxy_DX11()
	{	
		if( mpRasterizerState )
			mpRasterizerState->Release();
		mpRasterizerState = NULL;
	}

	bool GfxStateRasterizerProxy_DX11::Initialize(class GfxStateRasterizer& _Owner)
	{
		const GfxStateRasterizer::ResDataRef& rResData = _Owner.GetResData();
		ZENAssert(rResData.IsValid());
		ZENDbgCode(mpOwner = &_Owner);
		HRESULT hr = EMgr::GfxRender.DX11GetDevice()->CreateRasterizerState( &rResData->mRasterizerDesc, &mpRasterizerState );
		return SUCCEEDED(hr);
	}
}
