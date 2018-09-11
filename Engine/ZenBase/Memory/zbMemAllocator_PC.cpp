#include "zbBase.h"

namespace zbMem
{

Allocator_PC::Allocator_PC()
{	
	// Basic policy creates power2 pool sizes. 
	// Can be customized to fit specific needs and 'zenNewPool' creates new PoolGroup at runtime
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
	
	maPolicyAll[kPolicy__Unused]	= nullptr;
	maPolicyAll[kPolicy_Pool]		= &mPolicyPool;
	maPolicyAll[kPolicy_Regular]	= &mPolicyRegular;
	maPolicyAll[kPolicy_Large]		= &mPolicyLarge;
}

SAllocInfo Allocator_PC::MallocInternal(size_t _Size, size_t _MaxSize, bool _PoolItem, bool _IsCheckAccess)
{	
	if( _Size < kPoolItemSizeMax && _Size == _MaxSize && !_IsCheckAccess )
	{
		size_t AlignedSize = zenMath::RoundUp<size_t>(_Size, kAlign);
		if( _PoolItem && mPolicyPool.ShouldAddGroup(AlignedSize) )
		{
			PoolGroup::Config GroupConfig;
			GroupConfig.mItemSize				= AlignedSize;
			GroupConfig.mPageSize				= 4*1024; //! @todo 0 better heuristic
			GroupConfig.mPolicyType				= kPolicy_Pool;
			mPolicyPool.AddPoolGroup(GroupConfig);
		}		
		return mPolicyPool.Malloc(_Size);
	}
	else if( _MaxSize <= mPolicyRegular.GetSupportedSizeMax() )
	{
		return mPolicyRegular.Malloc(_Size);
	}		
	return mPolicyLarge.Malloc(_Size);
}

SAllocInfo Allocator_PC::ResizeInternal(void* _pMemory, size_t _NewSize)
{
	zenAssert(_pMemory != nullptr);
	VAddressCommonPC* pVAddressInfo = reinterpret_cast<VAddressCommonPC*>(&_pMemory);
	zenAssertMsg(maPolicyAll[pVAddressInfo->AllocatorType], "Unsupported MemoryPolicy, is the memory pointer valid?");
	return maPolicyAll[pVAddressInfo->AllocatorType]->Resize(_pMemory, _NewSize);
}

void Allocator_PC::FreeInternal(void* _pMemory)
{
	zenAssert(_pMemory != nullptr);
	VAddressCommonPC* pVAddressInfo = reinterpret_cast<VAddressCommonPC*>(&_pMemory);
	zenAssertMsg(maPolicyAll[pVAddressInfo->AllocatorType], "Unsupported MemoryPolicy, is the memory pointer valid?");
	maPolicyAll[pVAddressInfo->AllocatorType]->Free(_pMemory);
}



size_t Allocator_PC::GetRequestedSizeInternal(void* _pMemory)const
{
	zenAssert(_pMemory != nullptr);
	
	VAddressCommonPC* pVAddressInfo = reinterpret_cast<VAddressCommonPC*>(&_pMemory);
	zenAssertMsg(maPolicyAll[pVAddressInfo->AllocatorType], "Unsupported MemoryPolicy, is the memory pointer valid?");
	return maPolicyAll[pVAddressInfo->AllocatorType]->GetRequestedSize(_pMemory);
}

}  
