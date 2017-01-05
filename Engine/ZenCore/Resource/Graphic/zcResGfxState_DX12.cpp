#include "zcCore.h"

//SF DX12

namespace zcRes
{
	//////////////////////////////////////////////////////////////////////////
	// GfxSamplerHAL
	//////////////////////////////////////////////////////////////////////////
	GfxSamplerHAL_DX12::~GfxSamplerHAL_DX12()
	{
		if( mpSamplerState )
			mpSamplerState->Release();
		mpSamplerState = nullptr;
	}
	bool GfxSamplerHAL_DX12::Initialize()
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
	GfxStateBlendHAL_DX12::~GfxStateBlendHAL_DX12()
	{	
		if( mpBlendState )
			mpBlendState->Release();
		mpBlendState = nullptr;
	}

	bool GfxStateBlendHAL_DX12::Initialize()
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
	GfxStateDepthStencilHAL_DX12::~GfxStateDepthStencilHAL_DX12()
	{	
		if( mpDepthStencilState )
			mpDepthStencilState->Release();
		mpDepthStencilState = nullptr;
	}

	bool GfxStateDepthStencilHAL_DX12::Initialize()
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
	GfxStateRasterHAL_DX12::~GfxStateRasterHAL_DX12()
	{	
		if( mpRasterizerState )
			mpRasterizerState->Release();
		mpRasterizerState = nullptr;
	}

	bool GfxStateRasterHAL_DX12::Initialize()
	{
	#if ZEN_RENDERER_DX12
		return false;
	#else
		HRESULT hr			= zcMgr::GfxRender.DX12GetDevice()->CreateRasterizerState( &mRasterizerDesc, &mpRasterizerState );
		return SUCCEEDED(hr);
	#endif
	}
}
