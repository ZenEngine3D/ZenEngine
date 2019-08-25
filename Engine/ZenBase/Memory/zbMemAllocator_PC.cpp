#include "zbBase.h"

namespace zbMem
{

Allocator_PC::Allocator_PC()
{	
	// Basic policy creates pow2 pool sizes. 
	// Can be customized to fit specific needs and 'zenMem::NewPool' creates new PoolGroup at runtime
	size_t GroupItemSize(kAlign + mDebugTracking.GetSizeOverhead()), GroupCount(0);
	while( GroupItemSize <= kPoolItemSizeMax )
	{	
		PoolGroup::Config GroupConfig;		
		GroupConfig.mItemSize				= GroupItemSize;
		GroupConfig.mPageSize				= 4*1024; //! @todo 0 better heuristic
		GroupConfig.mPolicyType				= kPolicy_Pool;
		GroupItemSize						= (kAlign<<(++GroupCount)) + mDebugTracking.GetSizeOverhead();
		mPolicyPool.AddPoolGroup(GroupConfig);
	}
	
	maPolicyAll[kPolicy_NativeMalloc]	= &mPolicyNative;
	maPolicyAll[kPolicy_Pool]			= &mPolicyPool;
	maPolicyAll[kPolicy_Regular]		= &mPolicyRegular;
	maPolicyAll[kPolicy_Large]			= &mPolicyLarge;
}

SAllocInfo Allocator_PC::MallocInternal(size_t inSize, size_t inItemCount, zenMem::AllocFlags inAllocFlags)
{	
	const zenMem::AllocFlags PoolUnsupportedFlags(zenMem::keFlag_Resize, zenMem::keFlag_ResizeLarge, zenMem::keFlag_Protected);
	if(inAllocFlags.Any(zenMem::keFlag_NativeMalloc))
	{
		SAllocInfo Alloc = mPolicyNative.Malloc(inSize, inItemCount, inAllocFlags);
		zenAssert( reinterpret_cast<VAddressCommonPC*>(&Alloc.pMemory)->AllocatorType == kPolicy_NativeMalloc ); //Don't have control on memory range returned by malloc but should be fine
		return Alloc;
	}
	else if(inSize < kPoolItemSizeMax && inItemCount == 1 && inAllocFlags.Any(PoolUnsupportedFlags) == false)
	{
		size_t AlignedSize = zenMath::RoundUp<size_t>(inSize, kAlign);
		if( inAllocFlags.Any(zenMem::keFlag_Pool) && mPolicyPool.ShouldAddGroup(AlignedSize) )
		{
			PoolGroup::Config GroupConfig;
			GroupConfig.mItemSize				= AlignedSize;
			GroupConfig.mPageSize				= 4*1024; //! @todo 0 better heuristic
			GroupConfig.mPolicyType				= kPolicy_Pool;
			mPolicyPool.AddPoolGroup(GroupConfig);
		}		
		return mPolicyPool.Malloc(inSize, inItemCount, inAllocFlags);
	}
	else if( inSize > mPolicyRegular.GetSupportedSizeMax() || inAllocFlags.Any(zenMem::keFlag_ResizeLarge) )
	{
		return mPolicyLarge.Malloc(inSize, inItemCount, inAllocFlags);
	}	
	return mPolicyRegular.Malloc(inSize, inItemCount, inAllocFlags);
}

SAllocInfo Allocator_PC::ResizeInternal(void* _pMemory, size_t _NewSize, size_t inItemCount)
{
	zenAssert(_pMemory != nullptr);
	VAddressCommonPC* pVAddressInfo = reinterpret_cast<VAddressCommonPC*>(&_pMemory);
	zenAssertMsg(maPolicyAll[pVAddressInfo->AllocatorType], "Unsupported MemoryPolicy, is the memory pointer valid?");
	return maPolicyAll[pVAddressInfo->AllocatorType]->Resize(_pMemory, _NewSize, inItemCount);	
}

void Allocator_PC::FreeInternal(void* _pMemory)
{
	zenAssert(_pMemory != nullptr);
	VAddressCommonPC* pVAddressInfo = reinterpret_cast<VAddressCommonPC*>(&_pMemory);
	zenAssertMsg(maPolicyAll[pVAddressInfo->AllocatorType], "Unsupported MemoryPolicy, is the memory pointer valid?");
	maPolicyAll[pVAddressInfo->AllocatorType]->Free(_pMemory);
}

size_t Allocator_PC::GetItemCountInternal(void* _pMemory)const
{
	zenAssert(_pMemory != nullptr);
	VAddressCommonPC* pVAddressInfo = reinterpret_cast<VAddressCommonPC*>(&_pMemory);
	zenAssertMsg(maPolicyAll[pVAddressInfo->AllocatorType], "Unsupported MemoryPolicy, is the memory pointer valid?");
	return maPolicyAll[pVAddressInfo->AllocatorType]->GetRequestedCount(_pMemory);
}

}  
