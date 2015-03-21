#include "zcCore.h"

namespace zcRes
{

GfxShaderParamDef_DX11::GfxShaderParamDef_DX11()
: zcRes::Resource()
{
}

bool GfxShaderParamDef_DX11::ResourceInit()
{
	return TRUE;
}

GfxShaderParam_DX11::GfxShaderParam_DX11()
: zcRes::Resource()
, mbUpdated(false)
{	
}
	
GfxShaderParam_DX11::~GfxShaderParam_DX11()
{
	if(mInstanceInfo.mpBufferBinding)
		mInstanceInfo.mpBufferBinding->Release();
}

bool GfxShaderParam_DX11::ResourceInit()
{
	mInstanceInfo.mpParentShaderParamDef	= mInstanceInfo.mSerial.mParentParamDefID;
	mInstanceInfo.mpBufferBinding			= NULL;

	D3D11_BUFFER_DESC		bufferDesc;
	D3D11_SUBRESOURCE_DATA	initData;
	ZeroMemory( &bufferDesc, sizeof(bufferDesc) );
	bufferDesc.Usage			= D3D11_USAGE_DEFAULT;		
	bufferDesc.BindFlags		= D3D11_BIND_CONSTANT_BUFFER;
	bufferDesc.CPUAccessFlags	= 0;
	bufferDesc.ByteWidth		= mInstanceInfo.maParameterValues.Size();	
	initData.pSysMem			= mInstanceInfo.maParameterValues.First();
	initData.SysMemPitch		= 0;
	initData.SysMemSlicePitch	= 0;
	HRESULT hr					= EMgr::GfxRender.DX11GetDevice()->CreateBuffer( &bufferDesc, &initData, &mInstanceInfo.mpBufferBinding );		
	return SUCCEEDED(hr);
}

void GfxShaderParam_DX11::Bind(zcExp::eShaderStage _eShaderStage)const
{	
	ID3D11DeviceContext* pContext = EMgr::GfxRender.DX11GetDeviceContext();
	if( mbUpdated )
	{
		pContext->UpdateSubresource( mInstanceInfo.mpBufferBinding, 0, NULL, mInstanceInfo.maParameterValues.First(), 0, 0 );
		mbUpdated = FALSE;
	}

	switch( _eShaderStage )
	{
	case zcExp::keShaderStage_Vertex:	pContext->VSSetConstantBuffers( mInstanceInfo.mpParentShaderParamDef->Get().meFrequence, 1, &mInstanceInfo.mpBufferBinding );	break;
	case zcExp::keShaderStage_Pixel:		pContext->PSSetConstantBuffers( mInstanceInfo.mpParentShaderParamDef->Get().meFrequence, 1, &mInstanceInfo.mpBufferBinding );	break;	
	}
}

void GfxShaderParam_DX11::SetValue(const zcExp::ParameterBase& _Value)
{
	ZENAssert( mInstanceInfo.mpParentShaderParamDef.IsValid() )
	zcExp::ShaderParamItemInfo ItemInfo;
	if( mInstanceInfo.mpParentShaderParamDef->Get().mdParameters.Get(_Value.mhName, ItemInfo) )
	{
		mbUpdated = TRUE;
		zenMem::Copy( &mInstanceInfo.maParameterValues[ItemInfo.muOffset], _Value.GetData(), zenMath::Min(_Value.muSizeTotal, ItemInfo.muSize) );
	}	
}

void GfxShaderParam_DX11::SetValue(const zenRes::zShaderParameter& _Value)
{
	ZENAssert( mInstanceInfo.mpParentShaderParamDef.IsValid() )
	zcExp::ShaderParamItemInfo ItemInfo;
	if( mInstanceInfo.mpParentShaderParamDef->Get().mdParameters.Get(_Value.mhName, ItemInfo) )
	{
		mbUpdated = TRUE;
		zenMem::Copy( &mInstanceInfo.maParameterValues[ItemInfo.muOffset], _Value.GetData(), zenMath::Min(_Value.muSizeTotal, ItemInfo.muSize) );
	}	
}

void GfxShaderParam_DX11::SetValue(const zHash32& _hParamName, const float& _fValue)
{
	ZENAssert( mInstanceInfo.mpParentShaderParamDef.IsValid() )
	zcExp::ShaderParamItemInfo ItemInfo;
	if( mInstanceInfo.mpParentShaderParamDef->Get().mdParameters.Get(_hParamName, ItemInfo) )
	{	
		mbUpdated		= TRUE;
		ZENAssertMsg( ItemInfo.muSize >= sizeof(float), "Shader variable declared smaller than 1xFloat" );
		float* pValues	= reinterpret_cast<float*>(&mInstanceInfo.maParameterValues[ItemInfo.muOffset]);
		pValues[0]		= _fValue;		
	}	
}

void GfxShaderParam_DX11::SetValue(const zHash32& _hParamName, const zVec2F& _vValue)
{
	ZENAssert( mInstanceInfo.mpParentShaderParamDef.IsValid() )
	zcExp::ShaderParamItemInfo ItemInfo;
	if( mInstanceInfo.mpParentShaderParamDef->Get().mdParameters.Get(_hParamName, ItemInfo) )
	{			
		mbUpdated		= TRUE;
		ZENAssertMsg( ItemInfo.muSize >= sizeof(float)*2, "Shader variable declared smaller than 2xFloats" );
		float* pValues	= reinterpret_cast<float*>(&mInstanceInfo.maParameterValues[ItemInfo.muOffset]);
		pValues[0]		= _vValue.x;
		pValues[1]		= _vValue.y;
	}	
}

void GfxShaderParam_DX11::SetValue(const zHash32& _hParamName, const zVec3F& _vValue)
{
	ZENAssert( mInstanceInfo.mpParentShaderParamDef.IsValid() )
	zcExp::ShaderParamItemInfo ItemInfo;
	if( mInstanceInfo.mpParentShaderParamDef->Get().mdParameters.Get(_hParamName, ItemInfo) )
	{			
		mbUpdated		= TRUE;
		ZENAssertMsg( ItemInfo.muSize >= sizeof(float)*3, "Shader variable declared smaller than 3xFloats" );
		float* pValues	= reinterpret_cast<float*>(&mInstanceInfo.maParameterValues[ItemInfo.muOffset]);
		pValues[0]		= _vValue.x;
		pValues[1]		= _vValue.y;
		pValues[2]		= _vValue.z;
	}	
}

void GfxShaderParam_DX11::SetValue(const zHash32& _hParamName, const zVec4F& _vValue)
{
	ZENAssert( mInstanceInfo.mpParentShaderParamDef.IsValid() )			
	zcExp::ShaderParamItemInfo ItemInfo;
	if( mInstanceInfo.mpParentShaderParamDef->Get().mdParameters.Get(_hParamName, ItemInfo) )
	{			
		mbUpdated		= TRUE;
		ZENAssertMsg( ItemInfo.muSize >= sizeof(float)*4, "Shader variable declared smaller than 4xFloats" );
		float* pValues	= reinterpret_cast<float*>(&mInstanceInfo.maParameterValues[ItemInfo.muOffset]);
		pValues[0]		= _vValue.x;
		pValues[1]		= _vValue.y;
		pValues[2]		= _vValue.z;
		pValues[3]		= _vValue.w;
	}	
}

void GfxShaderParam_DX11::SetValue(const zHash32& _hParamName, const zenMath::Matrix& _matValue)
{
	ZENAssert( mInstanceInfo.mpParentShaderParamDef.IsValid() )			
	zcExp::ShaderParamItemInfo ItemInfo;
	if( mInstanceInfo.mpParentShaderParamDef->Get().mdParameters.Get(_hParamName, ItemInfo) )
	{			
		mbUpdated		= TRUE;
		ZENAssertMsg( ItemInfo.muSize >= sizeof(float)*16, "Shader variable declared smaller than 16xFloats" );
		zenMem::Copy( &mInstanceInfo.maParameterValues[ItemInfo.muOffset], &_matValue.mvRows[0], sizeof(float)*16 );
	}	
}

}