#include "zcCore.h"

namespace zcRes
{
//=================================================================================================
GfxShaderVertexHAL_DX11::~GfxShaderVertexHAL_DX11()
{
	if( mpVertexShader )
		mpVertexShader->Release();
	mpVertexShader = nullptr;
}

bool GfxShaderVertexHAL_DX11::Initialize()
{		
	HRESULT hr			= zcMgr::GfxRender.DX11GetDevice()->CreateVertexShader( maCompiledShader.First(), maCompiledShader.SizeMem(), nullptr, &mpVertexShader );
	return SUCCEEDED(hr);
}

//=================================================================================================
GfxShaderPixelHAL_DX11::~GfxShaderPixelHAL_DX11()
{
	if( mpPixelShader )
		mpPixelShader->Release();
	mpPixelShader = nullptr;
}

bool GfxShaderPixelHAL_DX11::Initialize()
{		
	HRESULT hr = zcMgr::GfxRender.DX11GetDevice()->CreatePixelShader( maCompiledShader.First(), maCompiledShader.SizeMem(), nullptr, &mpPixelShader );
	return SUCCEEDED(hr);
}

}

