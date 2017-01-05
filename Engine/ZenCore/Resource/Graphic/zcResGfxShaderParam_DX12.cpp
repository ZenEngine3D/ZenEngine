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
	if( mrBuffer.Get() != nullptr && mpCBufferMapped )
	{
		CD3DX12_RANGE WriteRange(0, 0);	//Nothing to write to gpu
		mrBuffer->Unmap(0, &WriteRange);
		mpCBufferMapped = nullptr;
	}
}

bool GfxCBufferHAL_DX12::Initialize()
{	
	mrCBufferParent		= mParentParamDefID;
	if( mrCBufferParent.IsValid() )
	{
		UINT uAlignedSize	= zenMath::RoundUp((UINT)maParameterValues.SizeMem(), UINT(256));
		HRESULT hr			= zcMgr::GfxRender.GetDevice()->CreateCommittedResource(	
								&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
								D3D12_HEAP_FLAG_NONE,
								&CD3DX12_RESOURCE_DESC::Buffer(uAlignedSize),
								D3D12_RESOURCE_STATE_GENERIC_READ,
								nullptr, IID_PPV_ARGS(&mrBuffer));	
		if( SUCCEEDED(hr) )
		{
			// Create constant buffer views to access the upload buffer.
			D3D12_CONSTANT_BUFFER_VIEW_DESC desc;
			mCBufferView		= zcGfx::DescriptorSRV_UAV_CBV::Allocate();						
			desc.BufferLocation = mrBuffer->GetGPUVirtualAddress();
			desc.SizeInBytes	= uAlignedSize;
			zcMgr::GfxRender.GetDevice()->CreateConstantBufferView(&desc, mCBufferView.GetHandle());

			// Map the constant buffers.
			CD3DX12_RANGE readRange(0, 0);		// We do not intend to read from this resource on the CPU.
			hr = mrBuffer->Map(0, &readRange, reinterpret_cast<void**>(&mpCBufferMapped));
			if( SUCCEEDED(hr) )
			{
				// We don't unmap this until the app closes. Keeping things mapped for the lifetime of the resource is okay.
				zenMem::Copy( mpCBufferMapped, maParameterValues.First(), maParameterValues.SizeMem() );
				return true;
			}
		}
	}
	return false;
}

void GfxCBufferHAL_DX12::Update( ID3D11DeviceContext& DirectXContext )
{
	if( mbUpdated && mpCBufferMapped )
	{
		zenMem::Copy( mpCBufferMapped, maParameterValues.First(), maParameterValues.SizeMem() );
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
