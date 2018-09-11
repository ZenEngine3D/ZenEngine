#include "zbBase.h"

namespace zbMem
{

//! @todo system to assign Index to each thread
zUInt GetThreadIndex()
{
	return 0;
}

void PoolGroup::Initialize(const PoolGroup::Config& _Config, zInt _GroupIndex)
{
	zenAssert(IsInitialized() == false);
	mConfig								= _Config;
	mConfig.mGroupIndex					= static_cast<decltype(_Config.mGroupIndex)>(_GroupIndex);
	mVirtualAddressStart.SubAddress		= 0;
	mVirtualAddressStart.SubAddress		= ~mVirtualAddressStart.SubAddress;
	mVirtualMemorySize					= mVirtualAddressStart.SubAddress + 1;	

	mVirtualAddressStart.SubAddress		= 0;
	mVirtualAddressStart._Unused		= 0;
	mVirtualAddressStart.Invalid		= 0;
	mVirtualAddressStart.GroupIndex		= mConfig.mGroupIndex;
	mVirtualAddressStart.PolicyType		= mConfig.mPolicyType;
	void* pResult						= VirtualAlloc(mVirtualAddressStart, mVirtualMemorySize, MEM_RESERVE, PAGE_READWRITE);
	zenAssert(pResult == mVirtualAddressStart);
}

bool PoolGroup::IsInitialized()const
{
	return mVirtualMemorySize != 0 && mConfig.mItemSize != 0;
}

SAllocInfo PoolGroup::Malloc(size_t _Size)
{
	zenAssert(IsInitialized() && _Size <= mConfig.mItemSize);
	if( mlstFree[GetThreadIndex()].IsEmpty() )
		GrowPool();
	
	zListElement* pNewItem = mlstFree[GetThreadIndex()].PopTail();
	return SAllocInfo(reinterpret_cast<void*>(pNewItem), mConfig.mItemSize);
}
void PoolGroup::Free(void* _pMemory)
{
	zenAssert(IsInitialized());
	zenAssert(_pMemory != nullptr)
	zListElement* pAvailableItem = new(_pMemory) zListElement;
	mlstFree[GetThreadIndex()].PushHead(*pAvailableItem);
}

void PoolGroup::GrowPool()
{
	zenAssert(mPhysicalAllocatedSize < mVirtualMemorySize);
	VAddressPool NewPageAddress = mVirtualAddressStart;
	NewPageAddress.SubAddress	= mPhysicalAllocatedSize;
	void* pResult				= VirtualAlloc(NewPageAddress, mConfig.mPageSize, MEM_COMMIT, PAGE_READWRITE);
	zenAssert(pResult == NewPageAddress);
	mPhysicalAllocatedSize		+= mConfig.mPageSize;
	
	zU8* pAddressCurrent		= reinterpret_cast<zU8*>(pResult);
	zU8* pAddressEnd			= pAddressCurrent + mConfig.mPageSize - mConfig.mItemSize;
	zListElement::List& lstFree	= mlstFree[GetThreadIndex()];
	while( pAddressCurrent <= pAddressEnd )
	{
		zListElement* pAvailableItem	= new(pAddressCurrent) zListElement;		
		pAddressCurrent					+= mConfig.mItemSize;
		lstFree.PushHead(*pAvailableItem);
	}
}

}  

