#include "zcCore.h"

namespace zcRes
{

GfxShaderParamDefProxy_DX11::GfxShaderParamDefProxy_DX11()
{
}

GfxShaderParamDefProxy_DX11::~GfxShaderParamDefProxy_DX11()
{
}

bool GfxShaderParamDefProxy_DX11::Initialize(class GfxShaderParamDef& _Owner)
{
	const GfxShaderParamDef::ResDataRef& rResData = _Owner.GetResData();
	ZENAssert(rResData.IsValid());
	ZENDbgCode(mpOwner = &_Owner);

	meFrequence			= rResData->meFrequence;
	mdParameters		= rResData->mdParameters;
	maParameterDefaults	= rResData->maParameterDefaults;
	return TRUE;
}


//=================================================================================================
GfxShaderParamProxy_DX11::GfxShaderParamProxy_DX11()
: mpBufferBinding(NULL)
, mbUpdated(false)
{	
}
	
GfxShaderParamProxy_DX11::~GfxShaderParamProxy_DX11()
{
	if(mpBufferBinding)
		mpBufferBinding->Release();
	mpBufferBinding = NULL;
}

bool GfxShaderParamProxy_DX11::Initialize(class GfxShaderParam& _Owner)
{
	D3D11_BUFFER_DESC		bufferDesc;
	D3D11_SUBRESOURCE_DATA	initData;
	ZeroMemory( &bufferDesc, sizeof(bufferDesc) );

	const GfxShaderParam::ResDataRef& rResData = _Owner.GetResData();
	ZENAssert(rResData.IsValid());
	ZENDbgCode(mpOwner = &_Owner);

	maParameterValues			= rResData->maParameterValues;
	mrProxShaderParamDef		= GetResourceProxy<GfxShaderParamDefRef>(rResData->mParentParamDefID);
	bufferDesc.Usage			= D3D11_USAGE_DEFAULT;		
	bufferDesc.BindFlags		= D3D11_BIND_CONSTANT_BUFFER;
	bufferDesc.CPUAccessFlags	= 0;
	bufferDesc.ByteWidth		= maParameterValues.Size();	
	initData.pSysMem			= maParameterValues.First();
	initData.SysMemPitch		= 0;
	initData.SysMemSlicePitch	= 0;
	HRESULT hr					= zcMgr::GfxRender.DX11GetDevice()->CreateBuffer( &bufferDesc, &initData, &mpBufferBinding );		
	return SUCCEEDED(hr);
}

void GfxShaderParamProxy_DX11::Bind(zenConst::eShaderStage _eShaderStage)const
{	
	ID3D11DeviceContext* pContext = zcMgr::GfxRender.DX11GetDeviceContext();
	if( mbUpdated )
	{
		pContext->UpdateSubresource( mpBufferBinding, 0, NULL, maParameterValues.First(), 0, 0 );
		mbUpdated = FALSE;
	}

	switch( _eShaderStage )
	{
	case zenConst::keShaderStage_Vertex:	pContext->VSSetConstantBuffers( mrProxShaderParamDef->meFrequence, 1, &mpBufferBinding );	break;
	case zenConst::keShaderStage_Pixel:		pContext->PSSetConstantBuffers( mrProxShaderParamDef->meFrequence, 1, &mpBufferBinding );	break;	
	}
}

void GfxShaderParamProxy_DX11::SetValue(const zcExp::ParameterBase& _Value)
{
	ZENAssert( mrProxShaderParamDef.IsValid() )
	zcExp::ShaderParamItemInfo ItemInfo;
	if( mrProxShaderParamDef->mdParameters.Get(_Value.mhName, ItemInfo) )
	{
		mbUpdated = TRUE;
		zenMem::Copy( &maParameterValues[ItemInfo.muOffset], _Value.GetData(), zenMath::Min(_Value.muSizeTotal, ItemInfo.muSize) );
	}	
}

void GfxShaderParamProxy_DX11::SetValue(const zenRes::zShaderParameter& _Value)
{
	ZENAssert( mrProxShaderParamDef.IsValid() )
	zcExp::ShaderParamItemInfo ItemInfo;
	if( mrProxShaderParamDef->mdParameters.Get(_Value.mhName, ItemInfo) )
	{
		mbUpdated = TRUE;
		zenMem::Copy( &maParameterValues[ItemInfo.muOffset], _Value.GetData(), zenMath::Min(_Value.muSizeTotal, ItemInfo.muSize) );
	}	
}

void GfxShaderParamProxy_DX11::SetValue(const zHash32& _hParamName, const float& _fValue)
{
	ZENAssert( mrProxShaderParamDef.IsValid() )
	zcExp::ShaderParamItemInfo ItemInfo;
	if( mrProxShaderParamDef->mdParameters.Get(_hParamName, ItemInfo) )
	{	
		mbUpdated		= TRUE;
		ZENAssertMsg( ItemInfo.muSize >= sizeof(float), "Shader variable declared smaller than 1xFloat" );
		float* pValues	= reinterpret_cast<float*>(&maParameterValues[ItemInfo.muOffset]);
		pValues[0]		= _fValue;		
	}	
}

void GfxShaderParamProxy_DX11::SetValue(const zHash32& _hParamName, const zVec2F& _vValue)
{
	ZENAssert( mrProxShaderParamDef.IsValid() )
	zcExp::ShaderParamItemInfo ItemInfo;
	if( mrProxShaderParamDef->mdParameters.Get(_hParamName, ItemInfo) )
	{			
		mbUpdated		= TRUE;
		ZENAssertMsg( ItemInfo.muSize >= sizeof(float)*2, "Shader variable declared smaller than 2xFloats" );
		float* pValues	= reinterpret_cast<float*>(&maParameterValues[ItemInfo.muOffset]);
		pValues[0]		= _vValue.x;
		pValues[1]		= _vValue.y;
	}	
}

void GfxShaderParamProxy_DX11::SetValue(const zHash32& _hParamName, const zVec3F& _vValue)
{
	ZENAssert( mrProxShaderParamDef.IsValid() )
	zcExp::ShaderParamItemInfo ItemInfo;
	if( mrProxShaderParamDef->mdParameters.Get(_hParamName, ItemInfo) )
	{			
		mbUpdated		= TRUE;
		ZENAssertMsg( ItemInfo.muSize >= sizeof(float)*3, "Shader variable declared smaller than 3xFloats" );
		float* pValues	= reinterpret_cast<float*>(&maParameterValues[ItemInfo.muOffset]);
		pValues[0]		= _vValue.x;
		pValues[1]		= _vValue.y;
		pValues[2]		= _vValue.z;
	}	
}

void GfxShaderParamProxy_DX11::SetValue(const zHash32& _hParamName, const zVec4F& _vValue)
{
	ZENAssert( mrProxShaderParamDef.IsValid() )			
	zcExp::ShaderParamItemInfo ItemInfo;
	if( mrProxShaderParamDef->mdParameters.Get(_hParamName, ItemInfo) )
	{			
		mbUpdated		= TRUE;
		ZENAssertMsg( ItemInfo.muSize >= sizeof(float)*4, "Shader variable declared smaller than 4xFloats" );
		float* pValues	= reinterpret_cast<float*>(&maParameterValues[ItemInfo.muOffset]);
		pValues[0]		= _vValue.x;
		pValues[1]		= _vValue.y;
		pValues[2]		= _vValue.z;
		pValues[3]		= _vValue.w;
	}	
}

void GfxShaderParamProxy_DX11::SetValue(const zHash32& _hParamName, const zenMath::Matrix& _matValue)
{
	ZENAssert( mrProxShaderParamDef.IsValid() )			
	zcExp::ShaderParamItemInfo ItemInfo;
	if( mrProxShaderParamDef->mdParameters.Get(_hParamName, ItemInfo) )
	{			
		mbUpdated		= TRUE;
		ZENAssertMsg( ItemInfo.muSize >= sizeof(float)*16, "Shader variable declared smaller than 16xFloats" );
		zenMem::Copy( &maParameterValues[ItemInfo.muOffset], &_matValue.mvRows[0], sizeof(float)*16 );
	}	
}



}