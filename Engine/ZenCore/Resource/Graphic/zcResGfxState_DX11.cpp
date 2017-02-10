#include "zcCore.h"

namespace zcRes
{
	//////////////////////////////////////////////////////////////////////////
	// GfxStateSamplerHAL
	//////////////////////////////////////////////////////////////////////////
	GfxStateSampler_DX11::~GfxStateSampler_DX11()
	{
		if( mpSamplerState )
			mpSamplerState->Release();
		mpSamplerState = nullptr;
	}
	bool GfxStateSampler_DX11::Initialize()
	{	
		HRESULT hr = zcMgr::GfxRender.GetDevice()->CreateSamplerState( &mSamplerDesc, &mpSamplerState );
		return SUCCEEDED(hr);
	}

	//////////////////////////////////////////////////////////////////////////
	//GfxBlend_DX11
	//////////////////////////////////////////////////////////////////////////
	GfxStateBlend_DX11::~GfxStateBlend_DX11()
	{	
		if( mpBlendState )
			mpBlendState->Release();
		mpBlendState = nullptr;
	}

	bool GfxStateBlend_DX11::Initialize()
	{
		HRESULT hr = zcMgr::GfxRender.GetDevice()->CreateBlendState( &mBlendDesc, &mpBlendState );
		//! @note : Global write mask / Blend factor : Unused functionality for now (per render target for now)
		muSampleMask = 0xFFFFFFFF;				
		for(zUInt i(0); i<zenArrayCount(mafBlendFactor); ++i)
			mafBlendFactor[i] = 0.0f;

		return SUCCEEDED(hr);
	}

	//////////////////////////////////////////////////////////////////////////
	//GfxDepthStencil_DX11
	//////////////////////////////////////////////////////////////////////////
	GfxStateDepthStencil_DX11::~GfxStateDepthStencil_DX11()
	{	
		if( mpDepthStencilState )
			mpDepthStencilState->Release();
		mpDepthStencilState = nullptr;
	}

	bool GfxStateDepthStencil_DX11::Initialize()
	{
		HRESULT hr = zcMgr::GfxRender.GetDevice()->CreateDepthStencilState( &mDepthStencilDesc, &mpDepthStencilState );
		muStencilValue = 0;
		return SUCCEEDED(hr);
	}

	//////////////////////////////////////////////////////////////////////////
	//GfxRasterizer_DX11
	//////////////////////////////////////////////////////////////////////////
	GfxStateRaster_DX11::~GfxStateRaster_DX11()
	{	
		if( mpRasterizerState )
			mpRasterizerState->Release();
		mpRasterizerState = nullptr;
	}

	bool GfxStateRaster_DX11::Initialize()
	{
		HRESULT hr			= zcMgr::GfxRender.GetDevice()->CreateRasterizerState( &mRasterizerDesc, &mpRasterizerState );
		return SUCCEEDED(hr);
	}
}
