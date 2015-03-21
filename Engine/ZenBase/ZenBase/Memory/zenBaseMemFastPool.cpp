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
	muItemSize			= zenMath::Max(sizeof(zList1xNode), _uItemSize);
	muItemIncrease		= _uItemIncrease;
	MemoryIncrease(_uItemCount);
}

void* zAllocatorFastPoolBase::Allocate()
{
	ZENAssertMsg( muItemSize > 0, "Trying to use an uninitialized pool");
	if( mlstFreeItems.IsEmpty() )
		MemoryIncrease( muItemIncrease );

	ZENAssert( !mlstFreeItems.IsEmpty() );
	void* pAllocatedItem = reinterpret_cast<void*>(mlstFreeItems.PopHead());
	return pAllocatedItem;
}

void zAllocatorFastPoolBase::Deallocate(void* _pAlloc)
{
	//! @todo safety Add debug check for integrity
	zList1xNode* pAllocFree = reinterpret_cast<zList1xNode*>(_pAlloc);
	mlstFreeItems.AddHead( pAllocFree );
}

void zAllocatorFastPoolBase::MemoryIncrease(zU32 _uItemCount)
{
	size_t			uSizeHeader		= zenMath::RoundUp(sizeof(MemAllocInfo), muItemSize);
	size_t			uTotalSize		= _uItemCount*muItemSize + uSizeHeader;
	MemAllocInfo*	pNewAlloc		= static_cast<MemAllocInfo*>(zMalloc(uTotalSize));
	zU8* pMemCur					= reinterpret_cast<zU8*>(pNewAlloc) + uSizeHeader;
	zU8* pMemEnd					= pMemCur + _uItemCount*muItemSize;
	while( pMemCur < pMemEnd )
	{
		mlstFreeItems.AddHead(reinterpret_cast<zList1xNode*>(pMemCur));
		pMemCur	+= muItemSize;
	}

	muReservedCount			+= _uItemCount;
	pNewAlloc->mDataSize	= _uItemCount*muItemSize;
	mlstAlloc.AddHead(pNewAlloc);
}

//==================================================================================================
//! @brief		Reset the memory reserved to 0
//==================================================================================================
void zAllocatorFastPoolBase::Clear()
{	
	ZENAssertMsg(GetAllocatedCount()==0, "Trying to clear a MemPool while there's still some items allocated." );
	muReservedCount	= 0;	
	mlstFreeItems.Clear();
	while( mlstAlloc.GetHead() != mlstAlloc.GetInvalid() )
	{
		zList1xNode* pDel = mlstAlloc.PopHead();
		zenDel(pDel);	
	}
}

} } //namespace zen { namespace zenMem
