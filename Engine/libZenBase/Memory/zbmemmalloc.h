#pragma once
#ifndef __LibCore_Memory_Malloc_h__
#define __LibCore_Memory_Malloc_h__

namespace CMem
{
	//=================================================================================================
	//! @class		MallocAllocator
	//-------------------------------------------------------------------------------------------------
	//! @brief		Basic malloc allocator the simply call the system malloc
	//! @details	Used as a temporary stand-in until more elaborate memory 
	//!				get implemented
	//=================================================================================================
	class MallocAllocator : public Allocator
	{
	AWClassDeclare(MallocAllocator, Allocator);
	public:
								MallocAllocator(const char* _zName);
		virtual void*			Malloc(size_t _uSize, bool _bIsArray, zenU32 _uAlign);
		virtual	void			Free(void* _pAlloc, AllocHeader* _pInfoAlloc);
		virtual zenDebugString	GetDescription(){return "Malloc";};
	};
}  

#endif
