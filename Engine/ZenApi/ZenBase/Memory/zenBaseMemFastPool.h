#pragma once
#ifndef __zenApi_Base_Memory_FastPool_h__
#define __zenApi_Base_Memory_FastPool_h__

namespace zen { namespace zenMem
{
//=================================================================================================
//! @class		zAllocatorFastPoolBase
//-------------------------------------------------------------------------------------------------
//! @brief		Pool allocator for fast allocations of generic data
//! @details	Faster than regular Pool allocator, but intended for simple data structure. 
//!				Has less safety / debugging feature than standard allocators.
//! @note		Constructor/Destructor of object not called
//! @note		Cannot be used with zenNew/zenDel
//=================================================================================================
class zAllocatorFastPoolBase 
{
zenClassDeclareNoParent(zAllocatorFastPoolBase);
public:
							zAllocatorFastPoolBase	();
							zAllocatorFastPoolBase	(size_t _uItemSize, zU32 _uItemCount, zU32 _uItemIncrease);
	virtual					~zAllocatorFastPoolBase	();
				void		Init(size_t _uItemSize, zU32 _uItemCount, zU32 _uItemIncrease);
				void		MemoryIncrease(zU32 _uItemCount);
				void		MemoryReleaseUnused();
	zenInline	size_t		GetItemSize()const;
	zenInline	size_t		GetReservedSize()const;
	zenInline	zU32		GetReservedCount()const;
	zenInline	zU32		GetIncreaseCount()const;
	zenInline	zU32		GetAllocatedCount()const;
	zenInline	void		SetIncreaseCount(zU32 _uIncreaseCount);
	
				void		Clear();
				void*		Allocate();
				void		Deallocate(void* _pAlloc);
protected:
	struct MemAllocInfo
	{
		size_t		mDataSize;
		zListLink	mlnkList;
		typedef zList<MemAllocInfo, &MemAllocInfo::mlnkList, false> TypeList;
	};
	struct PoolItem
	{
		zListLink mlnkList;
		typedef zList<PoolItem, &PoolItem::mlnkList, false> TypeList;
	};

	zU32					muAllocatedCount;	//!< Number of items currently allocated
	zU32					muReservedCount;	//!< Number of pool item reserved	
	size_t					muItemSize;			//!< Size of each pool item
	zU32					muItemIncrease;		//!< Amount of new item when pool runs out of item (0 for none)
	MemAllocInfo::TypeList	mlstAllocs;			//!< List of allocations done from mpAllocator to reserve space in the pool	
	PoolItem::TypeList		mlstFreeItems;		//!< List of the free pre-allocated items	
};

//=================================================================================================
//! @class		zAllocatorFastPool
//-------------------------------------------------------------------------------------------------
//! @brief		Pool allocator for fast allocations of structure data
//! @details	Faster than regular Pool allocator, but intended for simple data structure. 
//!				Has less safety / debugging feature than standard allocators.
//! @note		Not be safe to activate destructor without constructor on class with virtual
//! @note		Cannot be used with zenNew/zenDel
//=================================================================================================
template<class TObject, bool TCallConstructor=true, bool TCallDestructor=true>
class zAllocatorFastPool : public zAllocatorFastPoolBase
{
zenClassDeclare(zAllocatorFastPool, zAllocatorFastPoolBase);
public:
																	zAllocatorFastPool	();
																	zAllocatorFastPool	(zU32 _uItemCount, zU32 _uItemIncrease);
	void															Init				(zU32 _uItemCount, zU32 _uItemIncrease);	
	void															Deallocate			(TObject* _pAlloc);
	TObject*														Allocate			();	
	template<class TParam1> TObject*								Allocate			(TParam1);
	template<class TParam1, class TParam2> TObject*					Allocate			(TParam1, TParam2);
	template<class TParam1, class TParam2, class TParam3> TObject*	Allocate			(TParam1, TParam2, TParam3);
};

} } //namespace zen { namespace zenMem


#endif
