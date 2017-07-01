#pragma once

namespace zcGfx
{

template<D3D12_DESCRIPTOR_HEAP_TYPE TDescriptorType>
class DescriptorHeap;

//=================================================================================================
//! @details	Resource Descriptors binds a resource to the GPU. Similar concept to 
//!				shader resource view(SRV) in DX11. This object encapsulate a range of descriptors 
//=================================================================================================	
template<D3D12_DESCRIPTOR_HEAP_TYPE TDescriptorType>
struct TDescriptorRange
{
	zenInline										TDescriptorRange();
	zenInline										TDescriptorRange(TDescriptorRange&& _MoveCopy);
	
	zenInline										TDescriptorRange(const TDescriptorRange& _Source, zUInt _uOffset=0, zUInt _uCount=0);
	zenInline										~TDescriptorRange();
	zenInline TDescriptorRange&						operator=(TDescriptorRange&& _MoveCopy);

 	zenInline const D3D12_CPU_DESCRIPTOR_HANDLE&	GetCpu()const{return mHandleCpu;}
 	zenInline const D3D12_GPU_DESCRIPTOR_HANDLE&	GetGpu()const{return mHandleGpu;}
	zenInline D3D12_CPU_DESCRIPTOR_HANDLE			GetCpu(zUInt _uIndex)const;
	zenInline D3D12_GPU_DESCRIPTOR_HANDLE			GetGpu(zUInt _uIndex)const;
	zenInline void									Release();
	zenInline bool									IsValid()const;
	
	zenInline static void 							StaticInit();
protected:
	zenInline										TDescriptorRange(const D3D12_CPU_DESCRIPTOR_HANDLE _HandleCpu, D3D12_GPU_DESCRIPTOR_HANDLE _HandleGpu, zUInt _uCount);
	D3D12_CPU_DESCRIPTOR_HANDLE						mHandleCpu;
	D3D12_GPU_DESCRIPTOR_HANDLE						mHandleGpu;
	DescriptorHeap<TDescriptorType>*				mpOwnerHeap=nullptr;
	zenDbgCode(zUInt								muCount);			//!< Number of valid descriptors from this one. Only used for validation
	static zUInt									suDescriptorSize;	//!< Size of this Descriptor type
	friend class DescriptorHeap<TDescriptorType>;
};
using DescriptorRangeSRV		= TDescriptorRange<D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV>;
using DescriptorRangeRTV		= TDescriptorRange<D3D12_DESCRIPTOR_HEAP_TYPE_RTV>;
using DescriptorRangeDSV		= TDescriptorRange<D3D12_DESCRIPTOR_HEAP_TYPE_DSV>;
using DescriptorRangeSampler	= TDescriptorRange<D3D12_DESCRIPTOR_HEAP_TYPE_SAMPLER>;

extern DescriptorRangeSRV		gNullSRVView;
extern DescriptorRangeSRV		gNullUAVView;
extern DescriptorRangeSRV		gNullCBVView;

//=================================================================================================
//! @brief		Keep track of resource descriptor heap
//! @details	
//! @todo 1 Cleanup this once figure out how it work. Should use ref count?
//=================================================================================================	
template<D3D12_DESCRIPTOR_HEAP_TYPE TDescriptorType>
class DescriptorHeap
{
public:	
	using DescRange =	TDescriptorRange<TDescriptorType>;
															~DescriptorHeap();
	DescRange												Allocate(zUInt _uCount);
	void													Release(DescRange& _Descriptor);
	bool													Load( zUInt _uDescriptorCount, D3D12_DESCRIPTOR_HEAP_FLAGS _eFlags=D3D12_DESCRIPTOR_HEAP_FLAG_NONE );
	void													Unload();
	zenInline const DirectXComRef<ID3D12DescriptorHeap>&	GetHeap();
protected:
	DirectXComRef<ID3D12DescriptorHeap>	mrDXDescriptorHeap;
	DescRange							mDescriptorHead;
	zArrayBits							maDescriptorUsed;
	zUInt								muDescriptorCount = 0;	
};
using DescriptorHeapSRV		= DescriptorHeap<D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV>;
using DescriptorHeapRTV		= DescriptorHeap<D3D12_DESCRIPTOR_HEAP_TYPE_RTV>;
using DescriptorHeapDSV		= DescriptorHeap<D3D12_DESCRIPTOR_HEAP_TYPE_DSV>;
using DescriptorHeapSampler	= DescriptorHeap<D3D12_DESCRIPTOR_HEAP_TYPE_SAMPLER>;

}

#include "zcGfxResourceDescriptor_DX12.inl"
