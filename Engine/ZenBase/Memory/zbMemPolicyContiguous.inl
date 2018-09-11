#pragma once

namespace zbMem
{

template<class TVirtualAdrInfo, zU32 TAllocMax, zUInt TAllocatorType>
PolicyContiguous<TVirtualAdrInfo,TAllocMax,TAllocatorType>::PolicyContiguous()
{	
	TVirtualAdrInfo VirtualAdress;
	// It's tricky to get size of bitfield without capacity warning. This achieves it.
	VirtualAdress.SubAddress	= 0;	
	VirtualAdress.SubAddress	= ~VirtualAdress.SubAddress;
	mVirtualMemorySize			= VirtualAdress.SubAddress + 1;
	mPhysicalPageSize			= 4*1024; //SF @todo 0 fetch value
	
	VirtualAdress.SubAddress	= 0;
	VirtualAdress.Invalid		= 0;	
	VirtualAdress.AllocatorType	= TAllocatorType;	

	mAvailableHead				= 0;
	mAvailableTail				= 0;
	for(auto i(0); i<TAllocMax; ++i)
	{
		VirtualAdress.AllocationIndex	= i;
		void* MemoryStartResult			= VirtualAlloc(VirtualAdress, mVirtualMemorySize, MEM_RESERVE, PAGE_READWRITE);
		if( MemoryStartResult == VirtualAdress )
		{
			maAvailableIndices[mAvailableHead++] = i;
		}
	}
}

template<class TVirtualAdrInfo, zU32 TAllocMax, zUInt TAllocatorType>
SAllocInfo PolicyContiguous<TVirtualAdrInfo,TAllocMax,TAllocatorType>::Malloc(size_t _Size)
{
	zenAssert(_Size <= mVirtualMemorySize);
	zenAssertMsg( mAvailableTail+1 < mAvailableHead, "Ran out of allocation ranges" ); //Make sure there's more than 1 item left to allocate, else could run into multihreading issue
	TVirtualAdrInfo VirtualAdress;
	VirtualAdress.Invalid			= 0;
	VirtualAdress.SubAddress		= 0;
	VirtualAdress.AllocatorType		= TAllocatorType;
	zU32 FreeIndex					= mAvailableTail++ % TAllocMax;
	VirtualAdress.AllocationIndex	= maAvailableIndices[FreeIndex];

	auto& AllocatedInfo				= maAllocatedInfo[VirtualAdress.AllocationIndex];	
	AllocatedInfo.mSizeWanted		= _Size;
	AllocatedInfo.mSizeUsed	= zenMath::RoundUp(_Size, mPhysicalPageSize);

	void* MemoryStartResult			= VirtualAlloc(VirtualAdress, AllocatedInfo.mSizeUsed, MEM_COMMIT, PAGE_READWRITE);
	zenAssert( MemoryStartResult == VirtualAdress );
	return SAllocInfo(MemoryStartResult, AllocatedInfo.mSizeUsed);
}

template<class TVirtualAdrInfo, zU32 TAllocMax, zUInt TAllocatorType>
SAllocInfo PolicyContiguous<TVirtualAdrInfo,TAllocMax,TAllocatorType>::Resize(void* _pMemory, size_t _NewSize)
{
	zenAssert(_pMemory != nullptr);
	TVirtualAdrInfo* pVirtualAdress = reinterpret_cast<TVirtualAdrInfo*>(&_pMemory);
	auto& AllocatedInfo				= maAllocatedInfo[pVirtualAdress->AllocationIndex];
	AllocatedInfo.mSizeWanted		= _NewSize;
	size_t NeededAvailable			= zenMath::RoundUp(_NewSize, mPhysicalPageSize);

	if( NeededAvailable > AllocatedInfo.mSizeUsed )
	{
		TVirtualAdrInfo ResizeAddress	= *pVirtualAdress;
		ResizeAddress.SubAddress		= AllocatedInfo.mSizeUsed;
		void* MemoryStartResult			= VirtualAlloc(ResizeAddress, NeededAvailable-AllocatedInfo.mSizeUsed, MEM_COMMIT, PAGE_READWRITE);
		AllocatedInfo.mSizeUsed			= NeededAvailable;
		zenAssert( MemoryStartResult == ResizeAddress );
	}
	//! @todo 1 : Heuristic to avoid freeing memory rigth away?
	else if( NeededAvailable < AllocatedInfo.mSizeUsed )
	{
		TVirtualAdrInfo ResizeAddress	= *pVirtualAdress;
		ResizeAddress.SubAddress		= NeededAvailable;
		BOOL IsSuccess					= VirtualFree(ResizeAddress, AllocatedInfo.mSizeUsed-NeededAvailable, MEM_DECOMMIT);
		AllocatedInfo.mSizeUsed			= NeededAvailable;
		zenAssert( IsSuccess );
	}
	return SAllocInfo(_pMemory, AllocatedInfo.mSizeUsed);
}

template<class TVirtualAdrInfo, zU32 TAllocMax, zUInt TAllocatorType>
void PolicyContiguous<TVirtualAdrInfo,TAllocMax,TAllocatorType>::Free(void* _pMemory)
{
	zenAssert(_pMemory != nullptr);	
	VirtualFree(_pMemory, 0, MEM_DECOMMIT);	
	TVirtualAdrInfo* pVirtualAdress = reinterpret_cast<TVirtualAdrInfo*>(&_pMemory);	
	auto& AllocatedInfo				= maAllocatedInfo[pVirtualAdress->AllocationIndex];
	AllocatedInfo.mSizeUsed			= 0;
	AllocatedInfo.mSizeWanted		= 0;	
	zU32 FreeIndex					= mAvailableHead++ % TAllocMax;
	maAvailableIndices[FreeIndex]	= pVirtualAdress->AllocationIndex;	
}

template<class TVirtualAdrInfo, zU32 TAllocMax, zUInt TAllocatorType>
size_t PolicyContiguous<TVirtualAdrInfo,TAllocMax,TAllocatorType>::GetRequestedSize(void* _pMemory)
{
	TVirtualAdrInfo* pVirtualAdress = reinterpret_cast<TVirtualAdrInfo*>(&_pMemory);
	return maAllocatedInfo[pVirtualAdress->AllocationIndex].mSizeWanted;
}

template<class TVirtualAdrInfo, zU32 TAllocMax, zUInt TAllocatorType>
const typename PolicyContiguous<TVirtualAdrInfo,TAllocMax,TAllocatorType>::AllocInfo& PolicyContiguous<TVirtualAdrInfo,TAllocMax,TAllocatorType>::GetAllocInfo(void* _pMemory)const
{
	TVirtualAdrInfo* pVirtualAdress = reinterpret_cast<TVirtualAdrInfo*>(&_pMemory);
	return maAllocatedInfo[pVirtualAdress->AllocationIndex];
}

template<class TVirtualAdrInfo, zU32 TAllocMax, zUInt TAllocatorType>
size_t PolicyContiguous<TVirtualAdrInfo,TAllocMax,TAllocatorType>::GetSupportedSizeMax()const
{
	return mVirtualMemorySize;
}

}  
