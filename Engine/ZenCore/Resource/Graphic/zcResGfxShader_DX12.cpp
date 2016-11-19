#include "zcCore.h"

//SF DX12

namespace zcRes
{
//=================================================================================================
GfxShaderVertexHAL_DX12::~GfxShaderVertexHAL_DX12()
{
	if( mpVertexShader )
		mpVertexShader->Release();
	mpVertexShader = nullptr;
}

bool GfxShaderVertexHAL_DX12::Initialize()
{		
#if DISABLE_DX12
	return false;
#else
	HRESULT hr			= zcMgr::GfxRender.DX12GetDevice()->CreateVertexShader( maCompiledShader.First(), maCompiledShader.SizeMem(), nullptr, &mpVertexShader );
	return SUCCEEDED(hr);
#endif
}

//=================================================================================================
GfxShaderPixelHAL_DX12::~GfxShaderPixelHAL_DX12()
{
	if( mpPixelShader )
		mpPixelShader->Release();
	mpPixelShader = nullptr;
}

bool GfxShaderPixelHAL_DX12::Initialize()
{		
#if DISABLE_DX12
	return false;
#else
	HRESULT hr = zcMgr::GfxRender.DX12GetDevice()->CreatePixelShader( maCompiledShader.First(), maCompiledShader.SizeMem(), nullptr, &mpPixelShader );
	return SUCCEEDED(hr);
#endif
}

}

