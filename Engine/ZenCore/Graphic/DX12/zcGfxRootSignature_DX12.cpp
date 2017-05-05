#include "zcCore.h"

namespace zcGfx
{
D3D12_FEATURE_DATA_ROOT_SIGNATURE		RootSignature::sFeatureData;
zArrayStatic<D3D12_STATIC_SAMPLER_DESC>	RootSignature::saSamplerDescTemp;

bool RootSignature::StaticInitialize( )
{
	//! @todo 0 support sampler properly
	saSamplerDescTemp.SetCount(1);
	saSamplerDescTemp[0] = {};
	saSamplerDescTemp[0].Filter				= D3D12_FILTER_MIN_MAG_MIP_POINT;
	saSamplerDescTemp[0].AddressU			= D3D12_TEXTURE_ADDRESS_MODE_BORDER;
	saSamplerDescTemp[0].AddressV			= D3D12_TEXTURE_ADDRESS_MODE_BORDER;
	saSamplerDescTemp[0].AddressW			= D3D12_TEXTURE_ADDRESS_MODE_BORDER;
	saSamplerDescTemp[0].MipLODBias			= 0;
	saSamplerDescTemp[0].MaxAnisotropy		= 0;
	saSamplerDescTemp[0].ComparisonFunc		= D3D12_COMPARISON_FUNC_NEVER;
	saSamplerDescTemp[0].BorderColor		= D3D12_STATIC_BORDER_COLOR_TRANSPARENT_BLACK;
	saSamplerDescTemp[0].MinLOD				= 0.0f;
	saSamplerDescTemp[0].MaxLOD				= D3D12_FLOAT32_MAX;
	saSamplerDescTemp[0].ShaderRegister		= 0;
	saSamplerDescTemp[0].RegisterSpace		= 0;
	saSamplerDescTemp[0].ShaderVisibility	= D3D12_SHADER_VISIBILITY_PIXEL;

	zenMem::Zero(sFeatureData);
	sFeatureData.HighestVersion = D3D_ROOT_SIGNATURE_VERSION_1_1;	// This is the highest version the sample supports. If CheckFeatureSupport succeeds, the HighestVersion returned will not be greater than this.
	if (FAILED( zcMgr::GfxRender.GetDevice()->CheckFeatureSupport(D3D12_FEATURE_ROOT_SIGNATURE, &sFeatureData, sizeof(sFeatureData))))
	{
		sFeatureData.HighestVersion = D3D_ROOT_SIGNATURE_VERSION_1_0;
		if (FAILED( zcMgr::GfxRender.GetDevice()->CheckFeatureSupport(D3D12_FEATURE_ROOT_SIGNATURE, &sFeatureData, sizeof(sFeatureData))))
			return false;
	}
	return true;
}

RootSignature::RootSignature()
{	
}

RootSignature::RootSignature(const RootSignature& _Copy)
{
	mrRootSignature = _Copy.mrRootSignature;
}

RootSignature::RootSignature(const std::initializer_list<CD3DX12_ROOT_PARAMETER1>& _Entries, D3D12_ROOT_SIGNATURE_FLAGS _Flags)
{	
	CD3DX12_VERSIONED_ROOT_SIGNATURE_DESC RootDesc;
	RootDesc.Init_1_1((UINT)_Entries.size(), _Entries.begin(), (UINT)saSamplerDescTemp.Count(), saSamplerDescTemp.First(), _Flags); //No static sampler support for now

	DirectXComRef<ID3DBlob> rSignature, rError;
	HRESULT hr = D3DX12SerializeVersionedRootSignature(&RootDesc, sFeatureData.HighestVersion, rSignature.GetAddressOf(), rError.GetAddressOf());
	if( SUCCEEDED( hr) )
	{
		hr = zcMgr::GfxRender.GetDevice()->CreateRootSignature(0, rSignature->GetBufferPointer(), rSignature->GetBufferSize(), IID_PPV_ARGS(&mrRootSignature));
		//NAME_D3D12_OBJECT(mrRootSignature);
	}
}

RootSignature::RootSignature(const zArrayBase<CD3DX12_ROOT_PARAMETER1>& _Entries, D3D12_ROOT_SIGNATURE_FLAGS _Flags)
{	
	CD3DX12_VERSIONED_ROOT_SIGNATURE_DESC RootDesc;
	RootDesc.Init_1_1((UINT)_Entries.Count(), _Entries.First(), (UINT)saSamplerDescTemp.Count(), saSamplerDescTemp.First(), _Flags); //No static sampler support for now

	DirectXComRef<ID3DBlob> rSignature, rError;
	HRESULT hr = D3DX12SerializeVersionedRootSignature(&RootDesc, sFeatureData.HighestVersion, rSignature.GetAddressOf(), rError.GetAddressOf());
	if( SUCCEEDED( hr) )
	{
		hr = zcMgr::GfxRender.GetDevice()->CreateRootSignature(0, rSignature->GetBufferPointer(), rSignature->GetBufferSize(), IID_PPV_ARGS(&mrRootSignature));
		//NAME_D3D12_OBJECT(mrRootSignature);
	}
}

bool RootSignature::operator==(const RootSignature& _Cmp)const
{
	return mrRootSignature.Get() == _Cmp.Get();
}

bool RootSignature::operator!=(const RootSignature& _Cmp)const
{
	return mrRootSignature.Get() != _Cmp.Get();
}

}
