#include "zcCore.h"

//SF DX12

namespace zcRes
{
	//////////////////////////////////////////////////////////////////////////
	// GfxStateSamplerHAL
	//////////////////////////////////////////////////////////////////////////
	GfxStateSampler_DX12::~GfxStateSampler_DX12()
	{
		if( mpSamplerState )
			mpSamplerState->Release();
		mpSamplerState = nullptr;
	}
	bool GfxStateSampler_DX12::Initialize()
	{	
	#if ZEN_RENDERER_DX12
		return false;
	#else
		HRESULT hr = zcMgr::GfxRender.DX12GetDevice()->CreateSamplerState( &mSamplerDesc, &mpSamplerState );
		return SUCCEEDED(hr);
	#endif
	}

	//////////////////////////////////////////////////////////////////////////
	//GfxBlend_DX12
	//////////////////////////////////////////////////////////////////////////
	GfxStateBlend_DX12::~GfxStateBlend_DX12()
	{	
		if( mpBlendState )
			mpBlendState->Release();
		mpBlendState = nullptr;
	}

	bool GfxStateBlend_DX12::Initialize()
	{
	#if ZEN_RENDERER_DX12
		return false;
	#else
		HRESULT hr = zcMgr::GfxRender.DX12GetDevice()->CreateBlendState( &mBlendDesc, &mpBlendState );
		//! @note : Global write mask / Blend factor : Unused functionality for now (per render target for now)
		muSampleMask = 0xFFFFFFFF;				
		for(zUInt i(0); i<zenArrayCount(mafBlendFactor); ++i)
			mafBlendFactor[i] = 0.0f;

		return SUCCEEDED(hr);
	#endif
	}

	//////////////////////////////////////////////////////////////////////////
	//GfxDepthStencil_DX12
	//////////////////////////////////////////////////////////////////////////
	GfxStateDepthStencil_DX12::~GfxStateDepthStencil_DX12()
	{	
		if( mpDepthStencilState )
			mpDepthStencilState->Release();
		mpDepthStencilState = nullptr;
	}

	bool GfxStateDepthStencil_DX12::Initialize()
	{
	#if ZEN_RENDERER_DX12
		return false;
	#else
		HRESULT hr = zcMgr::GfxRender.DX12GetDevice()->CreateDepthStencilState( &mDepthStencilDesc, &mpDepthStencilState );
		muStencilValue = 0;
		return SUCCEEDED(hr);
	#endif
	}

	//////////////////////////////////////////////////////////////////////////
	//GfxRasterizer_DX12
	//////////////////////////////////////////////////////////////////////////
	GfxStateRaster_DX12::~GfxStateRaster_DX12()
	{	
		if( mpRasterizerState )
			mpRasterizerState->Release();
		mpRasterizerState = nullptr;
	}

	bool GfxStateRaster_DX12::Initialize()
	{
	#if ZEN_RENDERER_DX12
		return false;
	#else
		HRESULT hr			= zcMgr::GfxRender.DX12GetDevice()->CreateRasterizerState( &mRasterizerDesc, &mpRasterizerState );
		return SUCCEEDED(hr);
	#endif
	}
}
