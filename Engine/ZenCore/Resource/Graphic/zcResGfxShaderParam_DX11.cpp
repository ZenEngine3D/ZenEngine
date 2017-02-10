#include "zcCore.h"

namespace zcRes
{

bool GfxCBufferDefinition_DX11::Initialize()
{
	return TRUE;
}


//=================================================================================================

GfxCBuffer_DX11::~GfxCBuffer_DX11()
{
	if(mpBufferBinding)
		mpBufferBinding->Release();
	mpBufferBinding = nullptr;
}

bool GfxCBuffer_DX11::Initialize()
{
	D3D11_BUFFER_DESC		bufferDesc;
	D3D11_SUBRESOURCE_DATA	initData;
	ZeroMemory( &bufferDesc, sizeof(bufferDesc) );

	mrCBufferParent			= mParentParamDefID;
	bufferDesc.Usage			= D3D11_USAGE_DEFAULT;		
	bufferDesc.BindFlags		= D3D11_BIND_CONSTANT_BUFFER;
	bufferDesc.CPUAccessFlags	= 0;
	bufferDesc.ByteWidth		= static_cast<UINT>(maParameterValues.SizeMem());
	initData.pSysMem			= maParameterValues.First();
	initData.SysMemPitch		= 0;
	initData.SysMemSlicePitch	= 0;
	HRESULT hr					= zcMgr::GfxRender.GetDevice()->CreateBuffer( &bufferDesc, &initData, &mpBufferBinding );		
	return SUCCEEDED(hr) && mrCBufferParent.IsValid();
}

void GfxCBuffer_DX11::Update( ID3D11DeviceContext& DirectXContext )
{
	if( mbUpdated )
	{
		DirectXContext.UpdateSubresource( mpBufferBinding, 0, nullptr, maParameterValues.First(), 0, 0 );
		mbUpdated = FALSE;
	}
}

void GfxCBuffer_DX11::SetValue(const zcExp::ParameterBase& _Value)
{
	zcExp::GfxCBufferParamInfo ItemInfo;
	if( mrCBufferParent.HAL()->mdParamInfo.Get(_Value.mhName, ItemInfo) )
	{		
		zenMem::Copy( &maParameterValues[ItemInfo.muOffset], static_cast<const zU8*>(_Value.GetData()), zenMath::Min(_Value.muSizeTotal, ItemInfo.muSize) );
		mbUpdated = TRUE;
	}	
}

void GfxCBuffer_DX11::SetValue(const zenRes::zShaderParameter& _Value)
{
	zcExp::GfxCBufferParamInfo ItemInfo;
	if( mrCBufferParent.HAL()->mdParamInfo.Get(_Value.mhName, ItemInfo) )
	{		
		zenMem::Copy( &maParameterValues[ItemInfo.muOffset], static_cast<const zU8*>(_Value.GetData()), zenMath::Min(_Value.muSizeTotal, ItemInfo.muSize) );
		mbUpdated = TRUE;
	}	
}

}
