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
	virtual void*			Malloc				(size_t _uSize, bool _bIsArray, awU32 _uAlign=awDefaultAlign);
	virtual	void			Free				(void* _pAlloc, Header* _pInfoAlloc);
	virtual awDebugString		GetDescription()	{return awDebugString("Pool");}

public:
							PoolAllocator		();
							PoolAllocator		(awDebugString _zName, size_t _uItemSize, awU32 _uItemCount, awU32 _uItemIncrease, Allocator* _pAllocator=Allocator::Default, awU32 _uAlign=awDefaultAlign );
	virtual					~PoolAllocator		();
			void			Init				(awDebugString _zName, size_t _uItemSize, awU32 _uItemCount, awU32 _uItemIncrease, Allocator* _pAllocator=Allocator::Default, awU32 _uAlign=awDefaultAlign );
			void			MemoryIncrease		(awU32 _uItemCount);
			void			ReInit				();
			void			MemoryReleaseUnused	();
			size_t			GetItemSize			()const;
			size_t			GetReservedSize		()const;
			awU32				GetReservedCount	()const;
			awU32				GetIncreaseCount	()const;
			void			SetIncreaseCount	(awU32 _uIncreaseCount);
			Allocator*		GetAllocator		()const;
			void			Clear				();
protected:	
	awU32						mPoolReservedCount;	//!< Number of pool item reserved
	size_t					mPoolItemSize;		//!< Size of each pool item
	awU32						mPoolItemIncrease;	//!< Amount of new item when pool runs out of item (0 for none)
	awU32						mPoolItemAlign;		//!< Each item alignement
	awU32						mPoolItemCountInit; //!< Original pool item reserved
	awList1x					mlstFreeItems;		//!< List of the free pre-allocated items	
	awList1x					mlstAlloc;			//!< List of allocations done from mpAllocator to reserve space in the pool
	Allocator*				mpAllocator;		//!< Memory where the pools allocations reside
};
}  

//#include "CMemPool.inl"

#endif
