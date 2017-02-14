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
}
