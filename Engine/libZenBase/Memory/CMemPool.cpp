#include "libZenBase.h"

namespace CMem
{

PoolAllocator::PoolAllocator()
: Allocator( awDebugString("Uninitialized Pool"))
, mpAllocator(NULL)
{
}

PoolAllocator::PoolAllocator( awDebugString _zName, size_t _uItemSize, awU32 _uItemCount, awU32 _uItemIncrease, Allocator* _pAllocator, awU32 _uAlign )
: Allocator(_zName)
, mPoolReservedCount(0)
, mPoolItemSize(_uItemSize)
, mPoolItemIncrease(_uItemIncrease)
, mPoolItemAlign(_uAlign)
, mpAllocator(_pAllocator)
{
	MemoryIncrease(_uItemCount);
}

PoolAllocator::~PoolAllocator()
{
	Clear();
}

size_t PoolAllocator::GetItemSize()const
{
	return mPoolItemSize;
}

size_t PoolAllocator::GetReservedSize()const
{
	return mPoolReservedCount*mPoolItemSize;
}

awU32 PoolAllocator::GetReservedCount()const 
{
	return mPoolReservedCount;
}

awU32 PoolAllocator::GetIncreaseCount()const
{
	return mPoolItemIncrease;
}

void PoolAllocator::SetIncreaseCount(awU32 _uIncreaseCount)
{
	mPoolItemIncrease = _uIncreaseCount;
}

Allocator* PoolAllocator::GetAllocator()const
{
	return mpAllocator;
}

void PoolAllocator::Init(awDebugString _zName, size_t _uItemSize, awU32 _uItemCount, awU32 _uItemIncrease, Allocator* _pAllocator, awU32 _uAlign )
{
	AWAssertMsg(!mpAllocator, "Memory allocator already initialized.");
	AWAssertMsg(_uItemSize%_uAlign==0, "Pool item size must be a multiple of alignment specified.");
	mzAllocatorName		= _zName;
	mPoolReservedCount	= 0;
	mPoolItemSize		= _uItemSize;
	mPoolItemIncrease	= _uItemIncrease;
	mPoolItemAlign		= _uAlign;
	mpAllocator			= _pAllocator;
	mPoolItemCountInit	= _uItemCount;
	MemoryIncrease(_uItemCount);
}

void* PoolAllocator::Malloc(size_t _uSize, bool _bIsArray, awU32 _uAlign)
{
	AWAssertMsg(_uSize<=mPoolItemSize, "Pool configured for allocation of size %i, trying to allocate %i bytes", mPoolItemSize, _uSize );
	AWAssertMsg(_uAlign<=mPoolItemAlign, "Pool configured for alignment of %iBytes, requesting alignment of %i bytes", mPoolItemAlign, _uAlign );
	if( mlstFreeItems.IsEmpty() )
	{
		AWAssertMsg(mPoolItemIncrease, ("Pool ran out of memory and wasn't configured to auto increase size"));
		MemoryIncrease(mPoolReservedCount ? mPoolItemIncrease : mPoolItemCountInit);
	}
	
	awList1xNode* pItem = mlstFreeItems.PopHead();
	return AddAlloc(mPoolItemSize, 0, mPoolItemAlign, (void*)pItem, false);
}

void PoolAllocator::Free(void* _pAlloc, Header* _pInfoAlloc)
{
	AWAssert(_pAlloc && _pInfoAlloc);

	RemAlloc(_pInfoAlloc);
	awList1xNode* pPoolItemFree = (awList1xNode*)_pInfoAlloc;
	mlstFreeItems.AddHead(pPoolItemFree);	
}

void PoolAllocator::MemoryIncrease(awU32 _uItemCount)
{
	size_t			uPoolItemSize	= GetAllocSize(mPoolItemSize, 0, mPoolItemAlign);
	size_t			uTotalSize		= sizeof(awList1xNode)+_uItemCount*uPoolItemSize;	
	awList1xNode*	pNewAlloc		= (awList1xNode*)AWMalloc(mpAllocator, uTotalSize);
	awList1xNode*	pPoolFreeItem	= (awList1xNode*)(((awPointer)pNewAlloc)+sizeof(awList1xNode));
	void*			pMemEnd			= (void*)((awPointer)pNewAlloc + uTotalSize);
	mPoolReservedCount				+= _uItemCount;
	mlstAlloc.AddHead(pNewAlloc);
	while( pPoolFreeItem < pMemEnd )
	{
		mlstFreeItems.AddHead(pPoolFreeItem);
		pPoolFreeItem = (awList1xNode*)(((awPointer)pPoolFreeItem)+uPoolItemSize);
	}
}

//==================================================================================================
//! @brief		Reset the memory reserved to 0
//==================================================================================================
void PoolAllocator::Clear()
{	
	AWAssertMsg(GetTotalAllocCount()==0, "Trying to clear a MemPool while there's still some items allocated." );
	mPoolReservedCount	= 0;
	mlstFreeItems		= awList1x();	
	while( mlstAlloc.GetHead() != mlstAlloc.GetInvalid() )
	{
		awList1xNode* pDel = mlstAlloc.PopHead();
		AWDel(pDel);	
	}
}

}  
