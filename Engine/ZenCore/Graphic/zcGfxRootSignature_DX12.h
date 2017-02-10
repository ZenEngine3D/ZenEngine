#pragma once

namespace zcGfx
{

//! @note Inspired by : MiniEngine\Core\RootSignature.h

#if 0
class RootParameter
{
	friend class RootSignature;
public:

	RootParameter() 
	{
		m_RootParam.ParameterType = (D3D12_ROOT_PARAMETER_TYPE)0xFFFFFFFF;
	}

	~RootParameter()
	{
		Clear();
	}

	void Clear()
	{
		if (m_RootParam.ParameterType == D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE)
			delete [] m_RootParam.DescriptorTable.pDescriptorRanges;

		m_RootParam.ParameterType = (D3D12_ROOT_PARAMETER_TYPE)0xFFFFFFFF;
	}

	void InitAsConstants( UINT Register, UINT NumDwords, D3D12_SHADER_VISIBILITY Visibility = D3D12_SHADER_VISIBILITY_ALL )
	{
		m_RootParam.ParameterType = D3D12_ROOT_PARAMETER_TYPE_32BIT_CONSTANTS;
		m_RootParam.ShaderVisibility = Visibility;
		m_RootParam.Constants.Num32BitValues = NumDwords;
		m_RootParam.Constants.ShaderRegister = Register;
		m_RootParam.Constants.RegisterSpace = 0;
	}

	void InitAsConstantBuffer( UINT Register, D3D12_SHADER_VISIBILITY Visibility = D3D12_SHADER_VISIBILITY_ALL )
	{
		m_RootParam.ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
		m_RootParam.ShaderVisibility = Visibility;
		m_RootParam.Descriptor.ShaderRegister = Register;
		m_RootParam.Descriptor.RegisterSpace = 0;
	}

	void InitAsBufferSRV( UINT Register, D3D12_SHADER_VISIBILITY Visibility = D3D12_SHADER_VISIBILITY_ALL )
	{
		m_RootParam.ParameterType = D3D12_ROOT_PARAMETER_TYPE_SRV;
		m_RootParam.ShaderVisibility = Visibility;
		m_RootParam.Descriptor.ShaderRegister = Register;
		m_RootParam.Descriptor.RegisterSpace = 0;
	}

	void InitAsBufferUAV( UINT Register, D3D12_SHADER_VISIBILITY Visibility = D3D12_SHADER_VISIBILITY_ALL )
	{
		m_RootParam.ParameterType = D3D12_ROOT_PARAMETER_TYPE_UAV;
		m_RootParam.ShaderVisibility = Visibility;
		m_RootParam.Descriptor.ShaderRegister = Register;
		m_RootParam.Descriptor.RegisterSpace = 0;
	}

	void InitAsDescriptorRange( D3D12_DESCRIPTOR_RANGE_TYPE Type, UINT Register, UINT Count, D3D12_SHADER_VISIBILITY Visibility = D3D12_SHADER_VISIBILITY_ALL )
	{
		InitAsDescriptorTable(1, Visibility);
		SetTableRange(0, Type, Register, Count);
	}

	void InitAsDescriptorTable( UINT RangeCount, D3D12_SHADER_VISIBILITY Visibility = D3D12_SHADER_VISIBILITY_ALL )
	{
		m_RootParam.ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
		m_RootParam.ShaderVisibility = Visibility;
		m_RootParam.DescriptorTable.NumDescriptorRanges = RangeCount;
		m_RootParam.DescriptorTable.pDescriptorRanges = new D3D12_DESCRIPTOR_RANGE[RangeCount];
	}

	void SetTableRange( UINT RangeIndex, D3D12_DESCRIPTOR_RANGE_TYPE Type, UINT Register, UINT Count, UINT Space = 0 )
	{
		D3D12_DESCRIPTOR_RANGE* range = const_cast<D3D12_DESCRIPTOR_RANGE*>(m_RootParam.DescriptorTable.pDescriptorRanges + RangeIndex);
		range->RangeType = Type;
		range->NumDescriptors = Count;
		range->BaseShaderRegister = Register;
		range->RegisterSpace = Space;
		range->OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;
	}

	const D3D12_ROOT_PARAMETER& operator() ( void ) const { return m_RootParam; }
		

protected:

	D3D12_ROOT_PARAMETER m_RootParam;
};

// Maximum 64 DWORDS divied up amongst all root parameters.
// Root constants = 1 DWORD * NumConstants
// Root descriptor (CBV, SRV, or UAV) = 2 DWORDs each
// Descriptor table pointer = 1 DWORD
// Static samplers = 0 DWORDS (compiled into shader)
class RootSignature
{
	friend class DynamicDescriptorHeap;

public:

	RootSignature( UINT NumRootParams = 0, UINT NumStaticSamplers = 0 ) : m_Finalized(FALSE), m_NumParameters(NumRootParams)
	{
		Reset(NumRootParams, NumStaticSamplers);
	}

	~RootSignature()
	{
	}

	static void DestroyAll(void);

	void Reset( UINT NumRootParams, UINT NumStaticSamplers = 0 )
	{
		if (NumRootParams > 0)
			m_ParamArray.reset(new RootParameter[NumRootParams]);
		else
			m_ParamArray = nullptr;
		m_NumParameters = NumRootParams;

		if (NumStaticSamplers > 0)
			m_SamplerArray.reset(new D3D12_STATIC_SAMPLER_DESC[NumStaticSamplers]);
		else
			m_SamplerArray = nullptr;
		m_NumSamplers = NumStaticSamplers;
		m_NumInitializedStaticSamplers = 0;
	}

	RootParameter& operator[] ( size_t EntryIndex )
	{
		ASSERT(EntryIndex < m_NumParameters);
		return m_ParamArray.get()[EntryIndex];
	}

	const RootParameter& operator[] ( size_t EntryIndex ) const
	{
		ASSERT(EntryIndex < m_NumParameters);
		return m_ParamArray.get()[EntryIndex];
	}

	void InitStaticSampler( UINT Register, const D3D12_SAMPLER_DESC& NonStaticSamplerDesc,
		D3D12_SHADER_VISIBILITY Visibility = D3D12_SHADER_VISIBILITY_ALL );

	void Finalize(const std::wstring& name, D3D12_ROOT_SIGNATURE_FLAGS Flags = D3D12_ROOT_SIGNATURE_FLAG_NONE);

	ID3D12RootSignature* GetSignature() const { return m_Signature; }

protected:

	BOOL m_Finalized;
	UINT m_NumParameters;
	UINT m_NumSamplers;
	UINT m_NumInitializedStaticSamplers;
	uint32_t m_DescriptorTableBitMap;		// One bit is set for root parameters that are (non-sampler) descriptor tables
	uint32_t m_DescriptorTableSize[16];		// Non-sampler descriptor tables need to know their descriptor count
	UINT m_MaxDescriptorCacheHandleCount;	// The sum of all non-sampler descriptor table counts
	std::unique_ptr<RootParameter[]> m_ParamArray;
	std::unique_ptr<D3D12_STATIC_SAMPLER_DESC[]> m_SamplerArray;
	ID3D12RootSignature* m_Signature;
};

#endif

//void InitAsDescriptorRange( D3D12_DESCRIPTOR_RANGE_TYPE Type, UINT Register, UINT Count, D3D12_SHADER_VISIBILITY Visibility = D3D12_SHADER_VISIBILITY_ALL )
//{	InitAsDescriptorTable(1, Visibility); SetTableRange(0, Type, Register, Count); }


// Maximum 64 DWORDS divied up amongst all root parameters.
// Root constants = 1 DWORD * NumConstants
// Root descriptor (CBV, SRV, or UAV) = 2 DWORDs each
// Descriptor table pointer = 1 DWORD
// Static samplers = 0 DWORDS (compiled into shader)
class RootSignature
{
zenClassDeclareNoParent(RootSignature);
public:
	
									RootSignature();
									RootSignature(const RootSignature& _Copy);
									RootSignature(const std::initializer_list<CD3DX12_ROOT_PARAMETER1>& _Entries, D3D12_ROOT_SIGNATURE_FLAGS _Flags=D3D12_ROOT_SIGNATURE_FLAG_NONE);
									RootSignature(const zArrayBase<CD3DX12_ROOT_PARAMETER1>& _Entries, D3D12_ROOT_SIGNATURE_FLAGS _Flags=D3D12_ROOT_SIGNATURE_FLAG_NONE);
		
	zenInline ID3D12RootSignature*	Get()const{ return mrRootSignature.Get();}
	bool							operator==(const RootSignature& _Cmp)const;
	bool							operator!=(const RootSignature& _Cmp)const;

	static bool						StaticInitialize();

	zenInline static CD3DX12_ROOT_PARAMETER1 SlotTable( UINT numDescriptorRanges, _In_reads_(numDescriptorRanges) const D3D12_DESCRIPTOR_RANGE1* pDescriptorRanges, D3D12_SHADER_VISIBILITY visibility = D3D12_SHADER_VISIBILITY_ALL)
    {
		CD3DX12_ROOT_PARAMETER1 ParamDesc;
        CD3DX12_ROOT_PARAMETER1::InitAsDescriptorTable(ParamDesc, numDescriptorRanges, pDescriptorRanges, visibility);
		return ParamDesc;
    }
    
    zenInline static CD3DX12_ROOT_PARAMETER1 SlotConstant( UINT num32BitValues, UINT shaderRegister, UINT registerSpace = 0, D3D12_SHADER_VISIBILITY visibility = D3D12_SHADER_VISIBILITY_ALL)
    {
		CD3DX12_ROOT_PARAMETER1 ParamDesc;
        CD3DX12_ROOT_PARAMETER1::InitAsConstants(ParamDesc, num32BitValues, shaderRegister, registerSpace, visibility);
		return ParamDesc;
    }

    zenInline static CD3DX12_ROOT_PARAMETER1 SlotCBV( UINT shaderRegister, UINT registerSpace = 0, D3D12_ROOT_DESCRIPTOR_FLAGS flags = D3D12_ROOT_DESCRIPTOR_FLAG_NONE, D3D12_SHADER_VISIBILITY visibility = D3D12_SHADER_VISIBILITY_ALL)
    {
		CD3DX12_ROOT_PARAMETER1 ParamDesc;
        CD3DX12_ROOT_PARAMETER1::InitAsConstantBufferView(ParamDesc, shaderRegister, registerSpace, flags, visibility);
		return ParamDesc;
    }

    zenInline static CD3DX12_ROOT_PARAMETER1 SlotSRV(UINT shaderRegister, UINT registerSpace = 0, D3D12_ROOT_DESCRIPTOR_FLAGS flags = D3D12_ROOT_DESCRIPTOR_FLAG_NONE, D3D12_SHADER_VISIBILITY visibility = D3D12_SHADER_VISIBILITY_ALL)
    {
		CD3DX12_ROOT_PARAMETER1 ParamDesc;
        CD3DX12_ROOT_PARAMETER1::InitAsShaderResourceView(ParamDesc, shaderRegister, registerSpace, flags, visibility);
		return ParamDesc;
    }

    zenInline static CD3DX12_ROOT_PARAMETER1 SlotUAV( UINT shaderRegister, UINT registerSpace = 0, D3D12_ROOT_DESCRIPTOR_FLAGS flags = D3D12_ROOT_DESCRIPTOR_FLAG_NONE, D3D12_SHADER_VISIBILITY visibility = D3D12_SHADER_VISIBILITY_ALL)
    {
		CD3DX12_ROOT_PARAMETER1 ParamDesc;
        CD3DX12_ROOT_PARAMETER1::InitAsUnorderedAccessView(ParamDesc, shaderRegister, registerSpace, flags, visibility);
		return ParamDesc;
    }

protected:
	DirectXComRef<ID3D12RootSignature>				mrRootSignature;
	static D3D12_FEATURE_DATA_ROOT_SIGNATURE		sFeatureData;
	static zArrayStatic<D3D12_STATIC_SAMPLER_DESC>	saSamplerDescTemp;	//! @todo 3 implement sampler properly
};

}

