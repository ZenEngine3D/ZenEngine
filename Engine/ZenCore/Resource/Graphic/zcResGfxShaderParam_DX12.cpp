#include "zcCore.h"

//SF DX12
namespace zcRes
{

bool GfxCBufferDefinitionHAL_DX12::Initialize()
{
	return TRUE;
}


//=================================================================================================

GfxCBufferHAL_DX12::~GfxCBufferHAL_DX12()
{
	if(mpBufferBinding)
		mpBufferBinding->Release();
	mpBufferBinding = nullptr;
}

bool GfxCBufferHAL_DX12::Initialize()
{
#if DISABLE_DX12
	return false;
#else
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
	HRESULT hr					= zcMgr::GfxRender.DX12GetDevice()->CreateBuffer( &bufferDesc, &initData, &mpBufferBinding );		
	return SUCCEEDED(hr) && mrCBufferParent.IsValid();
#endif
}

void GfxCBufferHAL_DX12::Update( ID3D11DeviceContext& DirectXContext )
{
	if( mbUpdated )
	{
		DirectXContext.UpdateSubresource( mpBufferBinding, 0, nullptr, maParameterValues.First(), 0, 0 );
		mbUpdated = FALSE;
	}
}

void GfxCBufferHAL_DX12::SetValue(const zcExp::ParameterBase& _Value)
{
	zcExp::GfxCBufferParamInfo ItemInfo;
	if( mrCBufferParent.HAL()->mdParamInfo.Get(_Value.mhName, ItemInfo) )
	{		
		zenMem::Copy( &maParameterValues[ItemInfo.muOffset], static_cast<const zU8*>(_Value.GetData()), zenMath::Min(_Value.muSizeTotal, ItemInfo.muSize) );
		mbUpdated = TRUE;
	}	
}

void GfxCBufferHAL_DX12::SetValue(const zenRes::zShaderParameter& _Value)
{
	zcExp::GfxCBufferParamInfo ItemInfo;
	if( mrCBufferParent.HAL()->mdParamInfo.Get(_Value.mhName, ItemInfo) )
	{		
		zenMem::Copy( &maParameterValues[ItemInfo.muOffset], static_cast<const zU8*>(_Value.GetData()), zenMath::Min(_Value.muSizeTotal, ItemInfo.muSize) );
		mbUpdated = TRUE;
	}	
}

}
