#include "zbBase.h"

namespace zen { namespace zenMem
{

zAllocatorPool::zAllocatorPool()
: zenMem::zAllocator( zDebugString("Uninitialized Pool"))
, mPoolReservedCount(0)
, mPoolItemSize(0)
, mPoolItemIncrease(0)
, mPoolItemAlign(zenDefaultAlign)
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
	zenAssertMsg(_uItemSize%_uAlign==0, "Pool item size must be a multiple of alignment specified.");
	mzAllocatorName		= _zName;
	mPoolReservedCount	= 0;
	mPoolItemSize		= zenMath::Max(sizeof(PoolItem), _uItemSize);
	mPoolItemIncrease	= _uItemIncrease;
	mPoolItemAlign		= _uAlign;
	mPoolItemCountInit	= zenMath::Max<zU32>(1,_uItemCount);
	MemoryIncrease(_uItemCount);
}

void* zAllocatorPool::Malloc(size_t _uSize, bool _bIsArray, zU32 _uAlign)
{
	zenAssertMsg(_uSize<=mPoolItemSize, "Pool configured for allocation of size %i, trying to allocate %i bytes", mPoolItemSize, _uSize );
	zenAssertMsg(_uAlign<=mPoolItemAlign, "Pool configured for alignment of %iBytes, requesting alignment of %i bytes", mPoolItemAlign, _uAlign );
	if( mlstFreeItems.IsEmpty() )
	{
		zenAssertMsg(mPoolItemIncrease, ("Pool ran out of memory and wasn't configured to auto increase size"));
		MemoryIncrease(mPoolReservedCount ? mPoolItemIncrease : mPoolItemCountInit);
	}
	
	PoolItem* pItem = mlstFreeItems.PopHead();
	return AddAlloc(mPoolItemSize, 0, mPoolItemAlign, (void*)pItem, false);
}

void zAllocatorPool::Free(void* _pAlloc, void* _pInfoAlloc)
{
	zenAssert(_pAlloc && _pInfoAlloc);
	zbMem::AllocHeader* pInfoAlloc		= static_cast<zbMem::AllocHeader*>(_pInfoAlloc);
	RemAlloc(pInfoAlloc);

	PoolItem* pPoolItemFree				= reinterpret_cast<PoolItem*>(_pInfoAlloc);
	new(pPoolItemFree) PoolItem();
	mlstFreeItems.PushHead(*pPoolItemFree);	
}

void zAllocatorPool::MemoryIncrease(zU32 _uItemCount)
{
	size_t			uPoolItemSize	= GetAllocSize(mPoolItemSize, 0, mPoolItemAlign);
	size_t			uTotalSize		= sizeof(PoolAlloc)+_uItemCount*uPoolItemSize;
	PoolAlloc*		pNewAlloc		= reinterpret_cast<PoolAlloc*>(zMalloc(uTotalSize));	
	size_t			pMemCur			= reinterpret_cast<size_t>(pNewAlloc+1);
	size_t			pMemEnd			= reinterpret_cast<size_t>(pNewAlloc) + uTotalSize;
	
	while(pMemCur < pMemEnd)
	{
		PoolItem* pPoolItem = reinterpret_cast<PoolItem*>(pMemCur);
		new(pPoolItem) PoolItem();
		mlstFreeItems.PushHead(*pPoolItem);
		pMemCur += uPoolItemSize;
	}

	mPoolReservedCount += _uItemCount;
	new(pNewAlloc) PoolAlloc();
	mlstAlloc.PushHead(*pNewAlloc);
}

//==================================================================================================
//! @brief		Reset the memory reserved to 0
//==================================================================================================
void zAllocatorPool::Clear()
{	
	zenAssertMsg(GetTotalAllocCount()==0, "Trying to clear a MemPool while there's still some items allocated." );
	mPoolReservedCount	= 0;
	mlstFreeItems.Empty();
	while( mlstAlloc.GetHead() )
		zenDel(mlstAlloc.PopHead());	
}

} } //namespace zen { namespace zenMem
