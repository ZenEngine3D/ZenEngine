#include "zcCore.h"

namespace zcRes
{
	//////////////////////////////////////////////////////////////////////////
	// GfxSamplerHAL
	//////////////////////////////////////////////////////////////////////////
	GfxSamplerHAL_DX11::~GfxSamplerHAL_DX11()
	{
		if( mpSamplerState )
			mpSamplerState->Release();
		mpSamplerState = nullptr;
	}
	bool GfxSamplerHAL_DX11::Initialize()
	{	
		HRESULT hr = zcMgr::GfxRender.DX11GetDevice()->CreateSamplerState( &mSamplerDesc, &mpSamplerState );
		return SUCCEEDED(hr);
	}

	//////////////////////////////////////////////////////////////////////////
	//GfxBlend_DX11
	//////////////////////////////////////////////////////////////////////////
	GfxStateBlendHAL_DX11::~GfxStateBlendHAL_DX11()
	{	
		if( mpBlendState )
			mpBlendState->Release();
		mpBlendState = nullptr;
	}

	bool GfxStateBlendHAL_DX11::Initialize()
	{
		HRESULT hr = zcMgr::GfxRender.DX11GetDevice()->CreateBlendState( &mBlendDesc, &mpBlendState );
		//! @note : Global write mask / Blend factor : Unused functionality for now (per render target for now)
		muSampleMask = 0xFFFFFFFF;				
		for(zUInt i(0); i<zenArrayCount(mafBlendFactor); ++i)
			mafBlendFactor[i] = 0.0f;

		return SUCCEEDED(hr);
	}

	//////////////////////////////////////////////////////////////////////////
	//GfxDepthStencil_DX11
	//////////////////////////////////////////////////////////////////////////
	GfxStateDepthStencilHAL_DX11::~GfxStateDepthStencilHAL_DX11()
	{	
		if( mpDepthStencilState )
			mpDepthStencilState->Release();
		mpDepthStencilState = nullptr;
	}

	bool GfxStateDepthStencilHAL_DX11::Initialize()
	{
		HRESULT hr = zcMgr::GfxRender.DX11GetDevice()->CreateDepthStencilState( &mDepthStencilDesc, &mpDepthStencilState );
		muStencilValue = 0;
		return SUCCEEDED(hr);
	}

	//////////////////////////////////////////////////////////////////////////
	//GfxRasterizer_DX11
	//////////////////////////////////////////////////////////////////////////
	GfxStateRasterHAL_DX11::~GfxStateRasterHAL_DX11()
	{	
		if( mpRasterizerState )
			mpRasterizerState->Release();
		mpRasterizerState = nullptr;
	}

	bool GfxStateRasterHAL_DX11::Initialize()
	{
		HRESULT hr			= zcMgr::GfxRender.DX11GetDevice()->CreateRasterizerState( &mRasterizerDesc, &mpRasterizerState );
		return SUCCEEDED(hr);
	}
}
