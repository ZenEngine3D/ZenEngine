#pragma once

namespace zcGfx
{

//=================================================================================================
//! @brief		Keep track of resource descriptor heap
//! @details	
//! @todo 1 Cleanup this once figure out how it work. Should use ref count?
//=================================================================================================	
template<int TType, int THeapCount>
class ResourceDescriptor
{
public:
	enum constant { kuInvalid=0xFFFFFFFF, kuType=TType, kuHeapCount=THeapCount };
	bool IsValid()const
	{
		return muIndex != kuInvalid;
	}

	void Free()
	{
		if( IsValid() )
			saDescriptorUsed.Set(muIndex, false);
		muIndex = kuInvalid;
	}

	const D3D12_CPU_DESCRIPTOR_HANDLE& GetCpuHandle() const 
	{
		return mhDescriptor;
	}

	const D3D12_GPU_DESCRIPTOR_HANDLE& GetGpuHandle() const
	{
		return mhDescriptorGPU;
	}
	static ResourceDescriptor Allocate()
	{
		//! @todo 1 support bounds check
		ResourceDescriptor NewDesc;
		NewDesc.muIndex				= saDescriptorUsed.AddIndexTrue();
		NewDesc.mhDescriptor.ptr	= srDXCPUHandle.ptr + NewDesc.muIndex*suDescriptorSize;
		NewDesc.mhDescriptorGPU.ptr	= srDXGPUHandle.ptr + NewDesc.muIndex*suDescriptorSize;
		return NewDesc;
	}
	
	static const DirectXComRef<ID3D12DescriptorHeap>& GetDescHeap() {return srDXDescriptorHeap;}

	static bool Initialize( D3D12_DESCRIPTOR_HEAP_TYPE _eHeapType, D3D12_DESCRIPTOR_HEAP_FLAGS _eFlags=D3D12_DESCRIPTOR_HEAP_FLAG_NONE )
	{
		zenAssertMsg(suDescriptorSize == 0, "Resource Descriptor Heap already initialized");
		D3D12_DESCRIPTOR_HEAP_DESC HeapDesc={};
		HeapDesc.Type			= _eHeapType;
		HeapDesc.NumDescriptors = kuHeapCount;
		HeapDesc.Flags			= _eFlags;
		HeapDesc.NodeMask		= 0;
		HRESULT hr = zcMgr::GfxRender.GetDevice()->CreateDescriptorHeap(&HeapDesc, IID_PPV_ARGS(&srDXDescriptorHeap));
		if( FAILED( hr ) )
			return false;
		
		srDXCPUHandle			= srDXDescriptorHeap->GetCPUDescriptorHandleForHeapStart();
		srDXGPUHandle			= srDXDescriptorHeap->GetGPUDescriptorHandleForHeapStart();
		suDescriptorSize		= zcMgr::GfxRender.GetDevice()->GetDescriptorHandleIncrementSize(_eHeapType);
		saDescriptorUsed.SetRange(0, ResourceDescriptor::kuHeapCount, false);
		return true;
	}
protected:
	zUInt										muIndex	= kuInvalid;
	D3D12_CPU_DESCRIPTOR_HANDLE					mhDescriptor;
	D3D12_GPU_DESCRIPTOR_HANDLE					mhDescriptorGPU;

	static DirectXComRef<ID3D12DescriptorHeap>	srDXDescriptorHeap;
	static D3D12_CPU_DESCRIPTOR_HANDLE			srDXCPUHandle;
	static D3D12_GPU_DESCRIPTOR_HANDLE			srDXGPUHandle;
	static zUInt								suDescriptorSize;
	static zArrayBits							saDescriptorUsed;
	static 
	friend class ManagerRender;
	friend struct ResourceDescriptor2;
};

template<int TType, int THeapCount>	DirectXComRef<ID3D12DescriptorHeap> ResourceDescriptor<TType,THeapCount>::srDXDescriptorHeap;
template<int TType, int THeapCount>	D3D12_CPU_DESCRIPTOR_HANDLE			ResourceDescriptor<TType,THeapCount>::srDXCPUHandle;
template<int TType, int THeapCount>	D3D12_GPU_DESCRIPTOR_HANDLE			ResourceDescriptor<TType,THeapCount>::srDXGPUHandle;
template<int TType, int THeapCount>	zUInt								ResourceDescriptor<TType,THeapCount>::suDescriptorSize = 0;
template<int TType, int THeapCount>	zArrayBits							ResourceDescriptor<TType,THeapCount>::saDescriptorUsed;

using DescriptorRTV			= ResourceDescriptor<0, 128>;
using DescriptorDSV			= ResourceDescriptor<1, 64>;
using DescriptorSRV_UAV_CBV	= ResourceDescriptor<2, 1024>;

extern zcGfx::DescriptorSRV_UAV_CBV	gNullSRVView;
extern zcGfx::DescriptorSRV_UAV_CBV	gNullUAVView;
extern zcGfx::DescriptorSRV_UAV_CBV	gNullCBVView;

//! @todo 0 clean up naming
struct ResourceDescriptor2
{
	D3D12_CPU_DESCRIPTOR_HANDLE HandleCpu;
	D3D12_GPU_DESCRIPTOR_HANDLE HandleGpu;
	zenInline ResourceDescriptor2 Offset(zUInt _uIndex)const
	{
		ResourceDescriptor2 NewDesc;
		NewDesc.HandleCpu.ptr	= HandleCpu.ptr + _uIndex*DescriptorSRV_UAV_CBV::suDescriptorSize;
		NewDesc.HandleGpu.ptr	= HandleGpu.ptr + _uIndex*DescriptorSRV_UAV_CBV::suDescriptorSize;
		return NewDesc;
	};
};

}

#include "zcGfxResourceDescriptor_DX12.inl"

