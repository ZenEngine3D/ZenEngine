#pragma once
#ifndef __LibCore_Memory_Pool_h__
#define __LibCore_Memory_Pool_h__

namespace CMem
{
//=================================================================================================
//! @class		PoolAllocator
//-------------------------------------------------------------------------------------------------
//! @brief		Memory pool allocator, return quickly fixed size allocations
//! @details	Very fast allocator without fragmentation
//=================================================================================================
class PoolAllocator : public Allocator
{
AWClassDeclare(PoolAllocator, Allocator);
public:								
	virtual void*			Malloc				(size_t _uSize, bool _bIsArray, zenU32 _uAlign=awDefaultAlign);
	virtual	void			Free				(void* _pAlloc, Header* _pInfoAlloc);
	virtual zenDebugString		GetDescription()	{return zenDebugString("Pool");}

public:
							PoolAllocator		();
							PoolAllocator		(zenDebugString _zName, size_t _uItemSize, zenU32 _uItemCount, zenU32 _uItemIncrease, Allocator* _pAllocator=Allocator::Default, zenU32 _uAlign=awDefaultAlign );
	virtual					~PoolAllocator		();
			void			Init				(zenDebugString _zName, size_t _uItemSize, zenU32 _uItemCount, zenU32 _uItemIncrease, Allocator* _pAllocator=Allocator::Default, zenU32 _uAlign=awDefaultAlign );
			void			MemoryIncrease		(zenU32 _uItemCount);
			void			ReInit				();
			void			MemoryReleaseUnused	();
			size_t			GetItemSize			()const;
			size_t			GetReservedSize		()const;
			zenU32				GetReservedCount	()const;
			zenU32				GetIncreaseCount	()const;
			void			SetIncreaseCount	(zenU32 _uIncreaseCount);
			Allocator*		GetAllocator		()const;
			void			Clear				();
protected:	
	zenU32						mPoolReservedCount;	//!< Number of pool item reserved
	size_t					mPoolItemSize;		//!< Size of each pool item
	zenU32						mPoolItemIncrease;	//!< Amount of new item when pool runs out of item (0 for none)
	zenU32						mPoolItemAlign;		//!< Each item alignement
	zenU32						mPoolItemCountInit; //!< Original pool item reserved
	zenList1x					mlstFreeItems;		//!< List of the free pre-allocated items	
	zenList1x					mlstAlloc;			//!< List of allocations done from mpAllocator to reserve space in the pool
	Allocator*				mpAllocator;		//!< Memory where the pools allocations reside
};
}  

//#include "CMemPool.inl"

#endif
