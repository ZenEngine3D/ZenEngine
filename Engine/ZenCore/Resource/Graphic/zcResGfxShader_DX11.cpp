#include "zcCore.h"

namespace zcRes
{

//=================================================================================================
GfxInputSignatureProxy_DX11::GfxInputSignatureProxy_DX11()
{
}

GfxInputSignatureProxy_DX11::~GfxInputSignatureProxy_DX11()
{
}

bool GfxInputSignatureProxy_DX11::Initialize(class GfxInputSignature& _Owner)
{		
	ZENDbgCode(mpOwner = &_Owner);
	const GfxInputSignature::ResDataRef& rResData = _Owner.GetResData();	
	maDummyShaderCode									= rResData->maDummyShaderCode;
	return true;
}

//=================================================================================================
GfxShaderVertexProxy_DX11::GfxShaderVertexProxy_DX11()
: mpVertexShader(nullptr)
{
}

GfxShaderVertexProxy_DX11::~GfxShaderVertexProxy_DX11()
{
	if( mpVertexShader )
		mpVertexShader->Release();
	mpVertexShader = nullptr;
}

bool GfxShaderVertexProxy_DX11::Initialize(class GfxShaderVertex& _Owner)
{		
	ZENDbgCode(mpOwner = &_Owner);
	const GfxShaderVertex::ResDataRef& rResData = _Owner.GetResData();	
	HRESULT hr = zcMgr::GfxRender.DX11GetDevice()->CreateVertexShader( rResData->maCompiledShader.First(), rResData->maCompiledShader.SizeMem(), nullptr, &mpVertexShader );
	return SUCCEEDED(hr);
}

//=================================================================================================
GfxShaderPixelProxy_DX11::GfxShaderPixelProxy_DX11()
: mpPixelShader(nullptr)
{
}

GfxShaderPixelProxy_DX11::~GfxShaderPixelProxy_DX11()
{
	if( mpPixelShader )
		mpPixelShader->Release();
	mpPixelShader = nullptr;
}

bool GfxShaderPixelProxy_DX11::Initialize(class GfxShaderPixel& _Owner)
{		
	ZENDbgCode(mpOwner = &_Owner);
	const GfxShaderPixel::ResDataRef& rResData = _Owner.GetResData();	
	HRESULT hr = zcMgr::GfxRender.DX11GetDevice()->CreatePixelShader( rResData->maCompiledShader.First(), rResData->maCompiledShader.SizeMem(), nullptr, &mpPixelShader );
	return SUCCEEDED(hr);
}

}

