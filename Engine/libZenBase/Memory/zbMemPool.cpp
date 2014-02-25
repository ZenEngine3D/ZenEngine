#include "libZenBase.h"

namespace zbMem
{

PoolAllocator::PoolAllocator()
: zenMem::Allocator( zenDebugString("Uninitialized Pool"))
{
}

PoolAllocator::PoolAllocator( const zenDebugString& _zName, size_t _uItemSize, zenU32 _uItemCount, zenU32 _uItemIncrease, zenU32 _uAlign )
: zenMem::Allocator(_zName)
, mPoolReservedCount(0)
, mPoolItemSize(_uItemSize)
, mPoolItemIncrease(_uItemIncrease)
, mPoolItemAlign(_uAlign)
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

zenU32 PoolAllocator::GetReservedCount()const 
{
	return mPoolReservedCount;
}

zenU32 PoolAllocator::GetIncreaseCount()const
{
	return mPoolItemIncrease;
}

void PoolAllocator::SetIncreaseCount(zenU32 _uIncreaseCount)
{
	mPoolItemIncrease = _uIncreaseCount;
}

void PoolAllocator::Init(const zenDebugString& _zName, size_t _uItemSize, zenU32 _uItemCount, zenU32 _uItemIncrease, zenU32 _uAlign )
{
	AWAssertMsg(_uItemSize%_uAlign==0, "Pool item size must be a multiple of alignment specified.");
	mzAllocatorName		= _zName;
	mPoolReservedCount	= 0;
	mPoolItemSize		= _uItemSize;
	mPoolItemIncrease	= _uItemIncrease;
	mPoolItemAlign		= _uAlign;
	mPoolItemCountInit	= _uItemCount;
	MemoryIncrease(_uItemCount);
}

void* PoolAllocator::Malloc(size_t _uSize, bool _bIsArray, zenU32 _uAlign)
{
	AWAssertMsg(_uSize<=mPoolItemSize, "Pool configured for allocation of size %i, trying to allocate %i bytes", mPoolItemSize, _uSize );
	AWAssertMsg(_uAlign<=mPoolItemAlign, "Pool configured for alignment of %iBytes, requesting alignment of %i bytes", mPoolItemAlign, _uAlign );
	if( mlstFreeItems.IsEmpty() )
	{
		AWAssertMsg(mPoolItemIncrease, ("Pool ran out of memory and wasn't configured to auto increase size"));
		MemoryIncrease(mPoolReservedCount ? mPoolItemIncrease : mPoolItemCountInit);
	}
	
	zenList1xNode* pItem = mlstFreeItems.PopHead();
	return AddAlloc(mPoolItemSize, 0, mPoolItemAlign, (void*)pItem, false);
}

void PoolAllocator::Free(void* _pAlloc, void* _pInfoAlloc)
{
	AWAssert(_pAlloc && _pInfoAlloc);
	AllocHeader* pInfoAlloc = static_cast<AllocHeader*>(_pInfoAlloc);
	RemAlloc(pInfoAlloc);
	zenList1xNode* pPoolItemFree = (zenList1xNode*)_pInfoAlloc;
	mlstFreeItems.AddHead(pPoolItemFree);	
}

void PoolAllocator::MemoryIncrease(zenU32 _uItemCount)
{
	size_t			uPoolItemSize	= GetAllocSize(mPoolItemSize, 0, mPoolItemAlign);
	size_t			uTotalSize		= sizeof(zenList1xNode)+_uItemCount*uPoolItemSize;	
	zenList1xNode*	pNewAlloc		= (zenList1xNode*)zenMalloc(uTotalSize);
	zenList1xNode*	pPoolFreeItem	= (zenList1xNode*)(((zenPointer)pNewAlloc)+sizeof(zenList1xNode));
	void*			pMemEnd			= (void*)((zenPointer)pNewAlloc + uTotalSize);
	mPoolReservedCount				+= _uItemCount;
	mlstAlloc.AddHead(pNewAlloc);
	while( pPoolFreeItem < pMemEnd )
	{
		mlstFreeItems.AddHead(pPoolFreeItem);
		pPoolFreeItem = (zenList1xNode*)(((zenPointer)pPoolFreeItem)+uPoolItemSize);
	}
}

//==================================================================================================
//! @brief		Reset the memory reserved to 0
//==================================================================================================
void PoolAllocator::Clear()
{	
	AWAssertMsg(GetTotalAllocCount()==0, "Trying to clear a MemPool while there's still some items allocated." );
	mPoolReservedCount	= 0;
	mlstFreeItems		= zenList1x();	
	while( mlstAlloc.GetHead() != mlstAlloc.GetInvalid() )
	{
		zenList1xNode* pDel = mlstAlloc.PopHead();
		zenDel(pDel);	
	}
}

}  
