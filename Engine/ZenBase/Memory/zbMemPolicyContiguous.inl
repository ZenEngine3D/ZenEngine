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
SAllocInfo PolicyContiguous<TVirtualAdrInfo,TAllocMax,TAllocatorType>::Malloc(size_t inSize, size_t inItemCount, zenMem::AllocFlags inAllocFlags)
{
	zenAssert(inSize <= mVirtualMemorySize);
	zenAssertMsg( mAvailableTail+1 < mAvailableHead, "Ran out of allocation ranges" ); //Make sure there's more than 1 item left to allocate, else could run into multihreading issue
	TVirtualAdrInfo VirtualAdress;
	VirtualAdress.Invalid			= 0;
	VirtualAdress.SubAddress		= 0;
	VirtualAdress.AllocatorType		= TAllocatorType;
	zU32 FreeIndex					= mAvailableTail++ % TAllocMax;
	VirtualAdress.AllocationIndex	= maAvailableIndices[FreeIndex];

	AllocInfo& AllocatedInfo		= maAllocatedInfo[VirtualAdress.AllocationIndex];	
	AllocatedInfo.mSizeWanted		= static_cast<zU32>(inSize);
	AllocatedInfo.mSizeUsed			= static_cast<zU32>(zenMath::RoundUp(inSize, mPhysicalPageSize));
	AllocatedInfo.mItemCount		= static_cast<zU32>(inItemCount);
	AllocatedInfo.mFlags			= inAllocFlags;

	void* MemoryStartResult			= VirtualAlloc(VirtualAdress, AllocatedInfo.mSizeUsed, MEM_COMMIT, PAGE_READWRITE);
	zenAssert( MemoryStartResult == VirtualAdress );
	return SAllocInfo(MemoryStartResult, AllocatedInfo.mSizeUsed);
}

template<class TVirtualAdrInfo, zU32 TAllocMax, zUInt TAllocatorType>
SAllocInfo PolicyContiguous<TVirtualAdrInfo,TAllocMax,TAllocatorType>::Resize(void* inpMemory, size_t inNewSize, size_t inItemCount)
{
	zenAssert(inpMemory != nullptr);	
	TVirtualAdrInfo* pVirtualAdress = reinterpret_cast<TVirtualAdrInfo*>(&inpMemory);
	AllocInfo& AllocatedInfo		= maAllocatedInfo[pVirtualAdress->AllocationIndex];

	if( AllocatedInfo.mFlags.Any(zenMem::keFlag_Protected) )
		return Malloc(inNewSize, inItemCount, AllocatedInfo.mFlags); // When 'protect' active, memory at end of page (even if slower memcopy needed)

	AllocatedInfo.mSizeWanted		= static_cast<zU32>(inNewSize);
	AllocatedInfo.mItemCount		= static_cast<zU32>(inItemCount);
	size_t NeededAvailable			= static_cast<zU32>(zenMath::RoundUp(inNewSize, mPhysicalPageSize));

	if( NeededAvailable > AllocatedInfo.mSizeUsed )
	{
		TVirtualAdrInfo ResizeAddress	= *pVirtualAdress;
		ResizeAddress.SubAddress		= AllocatedInfo.mSizeUsed;
		void* MemoryStartResult			= VirtualAlloc(ResizeAddress, NeededAvailable-AllocatedInfo.mSizeUsed, MEM_COMMIT, PAGE_READWRITE);
		AllocatedInfo.mSizeUsed			= static_cast<zU32>(NeededAvailable);
		zenAssert( MemoryStartResult == ResizeAddress );
	}
	//! @todo 1 : Heuristic to avoid freeing memory right away?
	else if( NeededAvailable < AllocatedInfo.mSizeUsed )
	{
		TVirtualAdrInfo ResizeAddress	= *pVirtualAdress;
		ResizeAddress.SubAddress		= NeededAvailable;
		BOOL IsSuccess					= VirtualFree(ResizeAddress, AllocatedInfo.mSizeUsed-NeededAvailable, MEM_DECOMMIT);
		AllocatedInfo.mSizeUsed			= static_cast<zU32>(NeededAvailable);
		zenAssert( IsSuccess );
	}
	
	return SAllocInfo(inpMemory, AllocatedInfo.mSizeUsed);
}

template<class TVirtualAdrInfo, zU32 TAllocMax, zUInt TAllocatorType>
void PolicyContiguous<TVirtualAdrInfo,TAllocMax,TAllocatorType>::Free(void* inpMemory)
{
	zenAssert(inpMemory != nullptr);	
	VirtualFree(inpMemory, 0, MEM_DECOMMIT);	
	TVirtualAdrInfo* pVirtualAdress = reinterpret_cast<TVirtualAdrInfo*>(&inpMemory);	
	AllocInfo& AllocatedInfo		= maAllocatedInfo[pVirtualAdress->AllocationIndex];
	AllocatedInfo.mSizeUsed			= 0;
	AllocatedInfo.mSizeWanted		= 0;
	AllocatedInfo.mItemCount		= 0;
	zU32 FreeIndex					= mAvailableHead++ % TAllocMax;
	maAvailableIndices[FreeIndex]	= pVirtualAdress->AllocationIndex;	
}

template<class TVirtualAdrInfo, zU32 TAllocMax, zUInt TAllocatorType>
size_t PolicyContiguous<TVirtualAdrInfo,TAllocMax,TAllocatorType>::GetRequestedCount(void* inpMemory)
{
	TVirtualAdrInfo* pVirtualAdress = reinterpret_cast<TVirtualAdrInfo*>(&inpMemory);
	return maAllocatedInfo[pVirtualAdress->AllocationIndex].mItemCount;
}

template<class TVirtualAdrInfo, zU32 TAllocMax, zUInt TAllocatorType>
const typename PolicyContiguous<TVirtualAdrInfo,TAllocMax,TAllocatorType>::AllocInfo& PolicyContiguous<TVirtualAdrInfo,TAllocMax,TAllocatorType>::GetAllocInfo(void* inpMemory)const
{
	TVirtualAdrInfo* pVirtualAdress = reinterpret_cast<TVirtualAdrInfo*>(&inpMemory);
	return maAllocatedInfo[pVirtualAdress->AllocationIndex];
}

template<class TVirtualAdrInfo, zU32 TAllocMax, zUInt TAllocatorType>
size_t PolicyContiguous<TVirtualAdrInfo,TAllocMax,TAllocatorType>::GetSupportedSizeMax()const
{
	return mVirtualMemorySize;
}

}  
