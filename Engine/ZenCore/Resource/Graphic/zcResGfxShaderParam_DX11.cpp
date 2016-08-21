#include "zcCore.h"

namespace zcRes
{

bool GfxShaderParamDefHAL_DX11::Initialize()
{
	return TRUE;
}


//=================================================================================================

GfxShaderParamHAL_DX11::~GfxShaderParamHAL_DX11()
{
	if(mpBufferBinding)
		mpBufferBinding->Release();
	mpBufferBinding = nullptr;
}

bool GfxShaderParamHAL_DX11::Initialize()
{
	D3D11_BUFFER_DESC		bufferDesc;
	D3D11_SUBRESOURCE_DATA	initData;
	ZeroMemory( &bufferDesc, sizeof(bufferDesc) );

	mrShaderParamDef			= mParentParamDefID;
	bufferDesc.Usage			= D3D11_USAGE_DEFAULT;		
	bufferDesc.BindFlags		= D3D11_BIND_CONSTANT_BUFFER;
	bufferDesc.CPUAccessFlags	= 0;
	bufferDesc.ByteWidth		= maParameterValues.SizeMem();	
	initData.pSysMem			= maParameterValues.First();
	initData.SysMemPitch		= 0;
	initData.SysMemSlicePitch	= 0;
	HRESULT hr					= zcMgr::GfxRender.DX11GetDevice()->CreateBuffer( &bufferDesc, &initData, &mpBufferBinding );		
	return SUCCEEDED(hr) && mrShaderParamDef.IsValid();
}

void GfxShaderParamHAL_DX11::Bind(zenConst::eShaderStage _eShaderStage)const
{	
	ID3D11DeviceContext* pContext = zcMgr::GfxRender.DX11GetDeviceContext();
	if( mbUpdated )
	{
		pContext->UpdateSubresource( mpBufferBinding, 0, nullptr, maParameterValues.First(), 0, 0 );
		mbUpdated = FALSE;
	}

	switch( _eShaderStage )
	{
	case zenConst::keShaderStage_Vertex:	pContext->VSSetConstantBuffers( mrShaderParamDef.HAL()->meFrequence, 1, &mpBufferBinding );	break;
	case zenConst::keShaderStage_Pixel:		pContext->PSSetConstantBuffers( mrShaderParamDef.HAL()->meFrequence, 1, &mpBufferBinding );	break;
	}
}

void GfxShaderParamHAL_DX11::SetValue(const zcExp::ParameterBase& _Value)
{
	zcExp::ShaderParamItemInfo ItemInfo;
	if( mrShaderParamDef.HAL()->mdParameters.Get(_Value.mhName, ItemInfo) )
	{
		mbUpdated = TRUE;
		zenMem::Copy( &maParameterValues[ItemInfo.muOffset], static_cast<const zU8*>(_Value.GetData()), zenMath::Min(_Value.muSizeTotal, ItemInfo.muSize) );
	}	
}

void GfxShaderParamHAL_DX11::SetValue(const zenRes::zShaderParameter& _Value)
{
	zcExp::ShaderParamItemInfo ItemInfo;
	if( mrShaderParamDef.HAL()->mdParameters.Get(_Value.mhName, ItemInfo) )
	{
		mbUpdated = TRUE;
		zenMem::Copy( &maParameterValues[ItemInfo.muOffset], static_cast<const zU8*>(_Value.GetData()), zenMath::Min(_Value.muSizeTotal, ItemInfo.muSize) );
	}	
}

void GfxShaderParamHAL_DX11::SetValue(const zHash32& _hParamName, const float& _fValue)
{
	zcExp::ShaderParamItemInfo ItemInfo;
	if( mrShaderParamDef.HAL()->mdParameters.Get(_hParamName, ItemInfo) )
	{	
		mbUpdated		= TRUE;
		zenAssertMsg( ItemInfo.muSize >= sizeof(float), "Shader variable declared smaller than 1xFloat" );
		float* pValues	= reinterpret_cast<float*>(&maParameterValues[ItemInfo.muOffset]);
		pValues[0]		= _fValue;		
	}	
}

void GfxShaderParamHAL_DX11::SetValue(const zHash32& _hParamName, const zVec2F& _vValue)
{
	zcExp::ShaderParamItemInfo ItemInfo;
	if( mrShaderParamDef.HAL()->mdParameters.Get(_hParamName, ItemInfo) )
	{			
		mbUpdated		= TRUE;
		zenAssertMsg( ItemInfo.muSize >= sizeof(float)*2, "Shader variable declared smaller than 2xFloats" );
		float* pValues	= reinterpret_cast<float*>(&maParameterValues[ItemInfo.muOffset]);
		pValues[0]		= _vValue.x;
		pValues[1]		= _vValue.y;
	}	
}

void GfxShaderParamHAL_DX11::SetValue(const zHash32& _hParamName, const zVec3F& _vValue)
{
	zcExp::ShaderParamItemInfo ItemInfo;
	if( mrShaderParamDef.HAL()->mdParameters.Get(_hParamName, ItemInfo) )
	{			
		mbUpdated		= TRUE;
		zenAssertMsg( ItemInfo.muSize >= sizeof(float)*3, "Shader variable declared smaller than 3xFloats" );
		float* pValues	= reinterpret_cast<float*>(&maParameterValues[ItemInfo.muOffset]);
		pValues[0]		= _vValue.x;
		pValues[1]		= _vValue.y;
		pValues[2]		= _vValue.z;
	}	
}

void GfxShaderParamHAL_DX11::SetValue(const zHash32& _hParamName, const zVec4F& _vValue)
{
	zcExp::ShaderParamItemInfo ItemInfo;
	if( mrShaderParamDef.HAL()->mdParameters.Get(_hParamName, ItemInfo) )
	{			
		mbUpdated		= TRUE;
		zenAssertMsg( ItemInfo.muSize >= sizeof(float)*4, "Shader variable declared smaller than 4xFloats" );
		float* pValues	= reinterpret_cast<float*>(&maParameterValues[ItemInfo.muOffset]);
		pValues[0]		= _vValue.x;
		pValues[1]		= _vValue.y;
		pValues[2]		= _vValue.z;
		pValues[3]		= _vValue.w;
	}	
}

void GfxShaderParamHAL_DX11::SetValue(const zHash32& _hParamName, const zenMath::Matrix& _matValue)
{
	zcExp::ShaderParamItemInfo ItemInfo;
	if( mrShaderParamDef.HAL()->mdParameters.Get(_hParamName, ItemInfo) )
	{			
		mbUpdated		= TRUE;
		zenAssertMsg( ItemInfo.muSize >= sizeof(float)*16, "Shader variable declared smaller than 16xFloats" );
		zenMem::Copy( &maParameterValues[ItemInfo.muOffset], reinterpret_cast<const zU8*>(&_matValue.mvRows[0]), sizeof(float)*16 );
	}	
}

}
