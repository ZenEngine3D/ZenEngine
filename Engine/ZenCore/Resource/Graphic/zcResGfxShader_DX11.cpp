#include "zcCore.h"

namespace zcRes
{
//=================================================================================================
GfxShaderVertex_DX11::~GfxShaderVertex_DX11()
{
	if( mpVertexShader )
		mpVertexShader->Release();
	mpVertexShader = nullptr;
}

bool GfxShaderVertex_DX11::Initialize()
{		
	HRESULT hr			= zcMgr::GfxRender.GetDevice()->CreateVertexShader( maCompiledShader.Data(), maCompiledShader.SizeMem(), nullptr, &mpVertexShader );
	return SUCCEEDED(hr);
}

//=================================================================================================
GfxShaderPixel_DX11::~GfxShaderPixel_DX11()
{
	if( mpPixelShader )
		mpPixelShader->Release();
	mpPixelShader = nullptr;
}

bool GfxShaderPixel_DX11::Initialize()
{		
	HRESULT hr = zcMgr::GfxRender.GetDevice()->CreatePixelShader( maCompiledShader.Data(), maCompiledShader.SizeMem(), nullptr, &mpPixelShader );
	return SUCCEEDED(hr);
}

}

