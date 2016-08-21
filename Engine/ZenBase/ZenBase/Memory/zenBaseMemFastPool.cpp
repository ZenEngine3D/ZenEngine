#include "zbBase.h"

namespace zen {	namespace zenMem
{

zAllocatorFastPoolBase::zAllocatorFastPoolBase()
: muAllocatedCount(0)
, muReservedCount(0)
, muItemSize(0) 
, muItemIncrease(0)
{
}

zAllocatorFastPoolBase::zAllocatorFastPoolBase( size_t _uItemSize, zU32 _uItemCount, zU32 _uItemIncrease )
: muAllocatedCount(0)
, muReservedCount(0)
, muItemSize(0)
, muItemIncrease(0)
{
	Init( _uItemSize, _uItemCount, _uItemIncrease);
}

zAllocatorFastPoolBase::~zAllocatorFastPoolBase()
{
	Clear();
}


void zAllocatorFastPoolBase::Init( size_t _uItemSize, zU32 _uItemCount, zU32 _uItemIncrease )
{
	muAllocatedCount	= 0;
	muReservedCount		= 0;
	muItemSize			= zenMath::Max(sizeof(PoolItem), _uItemSize);
	muItemIncrease		= _uItemIncrease;
	MemoryIncrease(_uItemCount);
}

void* zAllocatorFastPoolBase::Allocate()
{
	zenAssertMsg( muItemSize > 0, "Trying to use an uninitialized pool");
	if( mlstFreeItems.IsEmpty() )
		MemoryIncrease( muItemIncrease );

	zenAssert( !mlstFreeItems.IsEmpty() );
	void* pAllocatedItem = reinterpret_cast<void*>(mlstFreeItems.PopHead());
	return pAllocatedItem;
}

void zAllocatorFastPoolBase::Deallocate(void* _pAlloc)
{
	//! @todo safety Add debug check for integrity
	PoolItem* pAllocFree = reinterpret_cast<PoolItem*>(_pAlloc);
	pAllocFree->mlnkList.SetNull();
	mlstFreeItems.PushHead(*pAllocFree);
}

void zAllocatorFastPoolBase::MemoryIncrease(zU32 _uItemCount)
{
	size_t			uSizeHeader		= zenMath::RoundUp(sizeof(MemAllocInfo), muItemSize);
	size_t			uTotalSize		= _uItemCount*muItemSize + uSizeHeader;
	MemAllocInfo*	pNewAlloc		= static_cast<MemAllocInfo*>(zMalloc(uTotalSize));	
	size_t pMemCur					= reinterpret_cast<size_t>(pNewAlloc) + uSizeHeader;
	size_t pMemEnd					= pMemCur + _uItemCount*muItemSize;

	while (pMemCur < pMemEnd)
	{
		PoolItem* pPoolItem = reinterpret_cast<PoolItem*>(pMemCur);
		pPoolItem->mlnkList.SetNull();
		mlstFreeItems.PushHead(*pPoolItem);
		pMemCur += muItemSize;
	}

	muReservedCount			+= _uItemCount;
	pNewAlloc->mDataSize	= _uItemCount*muItemSize;
	mlstAllocs.PushHead(*pNewAlloc);
}

//==================================================================================================
//! @brief		Reset the memory reserved to 0
//==================================================================================================
void zAllocatorFastPoolBase::Clear()
{	
	zenAssertMsg(GetAllocatedCount()==0, "Trying to clear a MemPool while there's still some items allocated." );
	muReservedCount	= 0;	
	mlstFreeItems.Empty();

	while( !mlstAllocs.IsEmpty() )
		zenDel(mlstAllocs.PopHead() );
}

} } //namespace zen { namespace zenMem
