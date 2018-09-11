#include "zcCore.h"

namespace zcGfx
{

template<D3D12_DESCRIPTOR_HEAP_TYPE TDescriptorType>
zUInt TDescriptorRange<TDescriptorType>::suDescriptorSize = 0;

//==================================================================================================
//! @detail:	Initialize the size of each descrptor for this type
//==================================================================================================
template<D3D12_DESCRIPTOR_HEAP_TYPE TDescriptorType>
void TDescriptorRange<TDescriptorType>::StaticInit()
{
	suDescriptorSize = zcMgr::GfxRender.GetDevice()->GetDescriptorHandleIncrementSize(TDescriptorType);
}

//==================================================================================================
//! @detail:	Empty Constructor
//==================================================================================================
template<D3D12_DESCRIPTOR_HEAP_TYPE TDescriptorType>
TDescriptorRange<TDescriptorType>::TDescriptorRange()
: mHandleCpu({0})
, mHandleGpu({0})
{
	zenDbgCode(muCount = 0);
}

//==================================================================================================
//! @detail:	Constructor used when first allocating descriptors from DX12 descriptors heaps
//--------------------------------------------------------------------------------------------------
//! @param _HandleCpu:	Value given to us by DirectX API
//! @param _HandleGpu:	Value given to us by DirectX API
//! @param _uCount:		Number of valid descriptors that was allocated
//==================================================================================================
template<D3D12_DESCRIPTOR_HEAP_TYPE TDescriptorType>
TDescriptorRange<TDescriptorType>::TDescriptorRange(const D3D12_CPU_DESCRIPTOR_HANDLE _HandleCpu, D3D12_GPU_DESCRIPTOR_HANDLE _HandleGpu, zUInt _uCount)
: mHandleCpu(_HandleCpu)
, mHandleGpu(_HandleGpu)
{
	zenDbgCode(muCount = _uCount);
}
//==================================================================================================
//! @detail:	Move Constructor
//--------------------------------------------------------------------------------------------------
//! @param _MoveCopy:	Object we are getting value from
//==================================================================================================
template<D3D12_DESCRIPTOR_HEAP_TYPE TDescriptorType>
TDescriptorRange<TDescriptorType>::TDescriptorRange(TDescriptorRange&& _MoveCopy)
: mHandleCpu(_MoveCopy.mHandleCpu)
, mHandleGpu(_MoveCopy.mHandleGpu)
, mpOwnerHeap(_MoveCopy.mpOwnerHeap)
{
	zenDbgCode(muCount		= _MoveCopy.muCount);
	_MoveCopy.mpOwnerHeap	= nullptr;
}

//==================================================================================================
//! @detail:	Move Assignment
//--------------------------------------------------------------------------------------------------
//! @param _MoveCopy:	Object we are getting value from
//==================================================================================================
template<D3D12_DESCRIPTOR_HEAP_TYPE TDescriptorType>
TDescriptorRange<TDescriptorType>& TDescriptorRange<TDescriptorType>::operator=(TDescriptorRange&& _MoveCopy)
{
	if(mpOwnerHeap)
		mpOwnerHeap->Release(*this);

	mHandleCpu				= _MoveCopy.mHandleCpu;
	mHandleGpu				= _MoveCopy.mHandleGpu;
	mpOwnerHeap				= _MoveCopy.mpOwnerHeap;
	zenDbgCode(muCount		= _MoveCopy.muCount);
	_MoveCopy.mpOwnerHeap	= nullptr;
	return *this;
}

//==================================================================================================
//! @detail:	Destructor. Release the descriptor range if allocated from a Heap
//==================================================================================================
template<D3D12_DESCRIPTOR_HEAP_TYPE TDescriptorType>
TDescriptorRange<TDescriptorType>::~TDescriptorRange()
{
	Release();	
}

//==================================================================================================
//! @detail:	Release the descriptor range if allocated from a Heap
//==================================================================================================
template<D3D12_DESCRIPTOR_HEAP_TYPE TDescriptorType>
void TDescriptorRange<TDescriptorType>::Release()
{
	if( mpOwnerHeap )
		mpOwnerHeap->Release(*this);
	mHandleCpu.ptr = 0;
	mHandleGpu.ptr = 0;
}

//==================================================================================================
//! @detail:	This object encapsulate a range of descriptors [1-muCount] and we can get 
//!				sub ranges of descriptors by using offsets.
//--------------------------------------------------------------------------------------------------
//! @param _Source:		Source descriptor to create this one from
//! @param _uOffset:	First valid element from Source that this object should point to
//! @param _uCount:		Number of valid descriptors should contain. Specify 0 if you want all 
//!						available descriptors of Source object, starting from offset.
//==================================================================================================
template<D3D12_DESCRIPTOR_HEAP_TYPE TDescriptorType>
TDescriptorRange<TDescriptorType>::TDescriptorRange(const TDescriptorRange& _Source, zUInt _uOffset, zUInt _uCount)
{
	zenAssert(_uOffset+_uCount <= _Source.muCount);
	mHandleCpu.ptr		= _Source.mHandleCpu.ptr + _uOffset*suDescriptorSize;
	mHandleGpu.ptr		= _Source.mHandleGpu.ptr + _uOffset*suDescriptorSize;
	zenDbgCode(muCount	= _uCount ? _uCount : _Source.muCount - _uOffset);
}

template<D3D12_DESCRIPTOR_HEAP_TYPE TDescriptorType>
D3D12_CPU_DESCRIPTOR_HANDLE TDescriptorRange<TDescriptorType>::GetCpu(zUInt _uIndex)const
{
	zenAssert(_uIndex < muCount);
	return D3D12_CPU_DESCRIPTOR_HANDLE({mHandleCpu.ptr + _uIndex*suDescriptorSize});
}

template<D3D12_DESCRIPTOR_HEAP_TYPE TDescriptorType>
D3D12_GPU_DESCRIPTOR_HANDLE TDescriptorRange<TDescriptorType>::GetGpu(zUInt _uIndex)const
{
	zenAssert(_uIndex < muCount);
	return D3D12_GPU_DESCRIPTOR_HANDLE({mHandleGpu.ptr + _uIndex*suDescriptorSize});
}

template<D3D12_DESCRIPTOR_HEAP_TYPE TDescriptorType>
bool TDescriptorRange<TDescriptorType>::IsValid()const
{
	return mHandleCpu.ptr != 0;
}

//==================================================================================================
//! @detail: Destructor. Returna descriptor heap to DirectX API
//==================================================================================================
template<D3D12_DESCRIPTOR_HEAP_TYPE TDescriptorType>
DescriptorHeap<TDescriptorType>::~DescriptorHeap()
{
	Unload();
}

//==================================================================================================
//! @detail:	Initialize a new descriptors heap, requesting directX api to create it for us	
//--------------------------------------------------------------------------------------------------
//! @param _uDescriptorCount:	Number of descirptors wanted in the heap
//! @param _eFlags:				DirectX flags used to create the heap
//==================================================================================================
template<D3D12_DESCRIPTOR_HEAP_TYPE TDescriptorType>
bool DescriptorHeap<TDescriptorType>::Load( zUInt _uDescriptorCount, D3D12_DESCRIPTOR_HEAP_FLAGS _eFlags )
{
	zenAssertMsg(muDescriptorCount == 0, "Resource Descriptor Heap already initialized");
	D3D12_DESCRIPTOR_HEAP_DESC HeapDesc={};
	muDescriptorCount		= _uDescriptorCount;
	HeapDesc.Type			= TDescriptorType;
	HeapDesc.NumDescriptors = (UINT)_uDescriptorCount;
	HeapDesc.Flags			= _eFlags;
	HeapDesc.NodeMask		= 0;
	HRESULT hr				= zcMgr::GfxRender.GetDevice()->CreateDescriptorHeap(&HeapDesc, IID_PPV_ARGS(&mrDXDescriptorHeap));
	if( FAILED( hr ) )
		return false;
	
	mDescriptorHead			= DescRange(mrDXDescriptorHeap->GetCPUDescriptorHandleForHeapStart(),
										mrDXDescriptorHeap->GetGPUDescriptorHandleForHeapStart(),
										muDescriptorCount );
	maDescriptorUsed.SetRange(0, muDescriptorCount, false);
	return true;
}

//==================================================================================================
//! @detail:	Release the DirectX descriptors Heap
//==================================================================================================
template<D3D12_DESCRIPTOR_HEAP_TYPE TDescriptorType>
void DescriptorHeap<TDescriptorType>::Unload()
{
	if( muDescriptorCount )
	{
		muDescriptorCount	= 0;
		mrDXDescriptorHeap	= nullptr;
		maDescriptorUsed.Reset(); //! @todo 3 safety add assert testing that all descrange have been released
	}
}

//==================================================================================================
//! @detail:		Find available descriptors range in the heap, with enough free entry to meet
//!					requested size.	
//--------------------------------------------------------------------------------------------------
//! @param _uCount: Number of descriptors wanted in the descriptors range
//==================================================================================================
template<D3D12_DESCRIPTOR_HEAP_TYPE TDescriptorType>
typename DescriptorHeap<TDescriptorType>::DescRange DescriptorHeap<TDescriptorType>::Allocate(zUInt _uCount)
{		
	zUInt uNewIndex		= (_uCount == 1) ? maDescriptorUsed.AddTrue() : maDescriptorUsed.AddRangeTrue(_uCount);
	DescRange NewDesc	= DescRange(mDescriptorHead, uNewIndex, _uCount);
	NewDesc.mpOwnerHeap	= this;
	zenAssertMsg(uNewIndex+_uCount <= muDescriptorCount, ("Allocated more descriptors than is available in this heap"));
	return NewDesc;
}

//==================================================================================================
//! @detail:	Let descriptor range with owner heap, release allocated descriptors 
//!				making them available again
//--------------------------------------------------------------------------------------------------
//! @param _Descriptor:		Resource Descriptor range to make available again
//==================================================================================================
template<D3D12_DESCRIPTOR_HEAP_TYPE TDescriptorType>
void DescriptorHeap<TDescriptorType>::Release(typename DescriptorHeap<TDescriptorType>::DescRange& _Descriptor)
{
	zUInt uIndex = (_Descriptor.GetCpu().ptr - mDescriptorHead.GetCpu().ptr) / _Descriptor.suDescriptorSize;
	zenAssert(uIndex+_Descriptor.muCount<=muDescriptorCount);
	zenAssert(_Descriptor.mpOwnerHeap == this);
	maDescriptorUsed.SetRange(uIndex, uIndex+_Descriptor.muCount-1, false);
	_Descriptor.mpOwnerHeap = nullptr;
}

template<D3D12_DESCRIPTOR_HEAP_TYPE TDescriptorType>
const DirectXComRef<ID3D12DescriptorHeap>& DescriptorHeap<TDescriptorType>::GetHeap()
{
	return mrDXDescriptorHeap;
}

}
