#include "libZenBase.h"

namespace zen { namespace zenMem
{

zAllocatorPool::zAllocatorPool()
: zenMem::zAllocator( zDebugString("Uninitialized Pool"))
{
}

zAllocatorPool::zAllocatorPool( const zDebugString& _zName, size_t _uItemSize, zU32 _uItemCount, zU32 _uItemIncrease, zU32 _uAlign )
: zenMem::zAllocator(_zName)
, mPoolReservedCount(0)
, mPoolItemSize(_uItemSize)
, mPoolItemIncrease(_uItemIncrease)
, mPoolItemAlign(_uAlign)
{
	MemoryIncrease(_uItemCount);
}

zAllocatorPool::~zAllocatorPool()
{
	Clear();
}

size_t zAllocatorPool::GetItemSize()const
{
	return mPoolItemSize;
}

size_t zAllocatorPool::GetReservedSize()const
{
	return mPoolReservedCount*mPoolItemSize;
}

zU32 zAllocatorPool::GetReservedCount()const 
{
	return mPoolReservedCount;
}

zU32 zAllocatorPool::GetIncreaseCount()const
{
	return mPoolItemIncrease;
}

void zAllocatorPool::SetIncreaseCount(zU32 _uIncreaseCount)
{
	mPoolItemIncrease = _uIncreaseCount;
}

void zAllocatorPool::Init(const zDebugString& _zName, size_t _uItemSize, zU32 _uItemCount, zU32 _uItemIncrease, zU32 _uAlign )
{
	ZENAssertMsg(_uItemSize%_uAlign==0, "Pool item size must be a multiple of alignment specified.");
	mzAllocatorName		= _zName;
	mPoolReservedCount	= 0;
	mPoolItemSize		= _uItemSize;
	mPoolItemIncrease	= _uItemIncrease;
	mPoolItemAlign		= _uAlign;
	mPoolItemCountInit	= _uItemCount;
	MemoryIncrease(_uItemCount);
}

void* zAllocatorPool::Malloc(size_t _uSize, bool _bIsArray, zU32 _uAlign)
{
	ZENAssertMsg(_uSize<=mPoolItemSize, "Pool configured for allocation of size %i, trying to allocate %i bytes", mPoolItemSize, _uSize );
	ZENAssertMsg(_uAlign<=mPoolItemAlign, "Pool configured for alignment of %iBytes, requesting alignment of %i bytes", mPoolItemAlign, _uAlign );
	if( mlstFreeItems.IsEmpty() )
	{
		ZENAssertMsg(mPoolItemIncrease, ("Pool ran out of memory and wasn't configured to auto increase size"));
		MemoryIncrease(mPoolReservedCount ? mPoolItemIncrease : mPoolItemCountInit);
	}
	
	zList1xNode* pItem = mlstFreeItems.PopHead();
	return AddAlloc(mPoolItemSize, 0, mPoolItemAlign, (void*)pItem, false);
}

void zAllocatorPool::Free(void* _pAlloc, void* _pInfoAlloc)
{
	ZENAssert(_pAlloc && _pInfoAlloc);
	zbMem::AllocHeader* pInfoAlloc = static_cast<zbMem::AllocHeader*>(_pInfoAlloc);
	RemAlloc(pInfoAlloc);
	zList1xNode* pPoolItemFree = (zList1xNode*)_pInfoAlloc;
	mlstFreeItems.AddHead(pPoolItemFree);	
}

void zAllocatorPool::MemoryIncrease(zU32 _uItemCount)
{
	size_t			uPoolItemSize	= GetAllocSize(mPoolItemSize, 0, mPoolItemAlign);
	size_t			uTotalSize		= sizeof(zList1xNode)+_uItemCount*uPoolItemSize;	
	zList1xNode*	pNewAlloc		= static_cast<zList1xNode*>(zMalloc(uTotalSize));
	zList1xNode*	pPoolFreeItem	= pNewAlloc+1;
	void*			pMemEnd			= (void*)((zU8*)pNewAlloc + uTotalSize);
	mPoolReservedCount				+= _uItemCount;
	mlstAlloc.AddHead(pNewAlloc);
	while( pPoolFreeItem < pMemEnd )
	{
		mlstFreeItems.AddHead(pPoolFreeItem);
		pPoolFreeItem = (zList1xNode*)(((zU8*)pPoolFreeItem)+uPoolItemSize);
	}
}

//==================================================================================================
//! @brief		Reset the memory reserved to 0
//==================================================================================================
void zAllocatorPool::Clear()
{	
	ZENAssertMsg(GetTotalAllocCount()==0, "Trying to clear a MemPool while there's still some items allocated." );
	mPoolReservedCount	= 0;
	mlstFreeItems		= zList1x();	
	while( mlstAlloc.GetHead() != mlstAlloc.GetInvalid() )
	{
		zList1xNode* pDel = mlstAlloc.PopHead();
		zenDel(pDel);	
	}
}

} } //namespace zen { namespace zenMem
