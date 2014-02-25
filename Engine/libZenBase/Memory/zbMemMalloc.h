#pragma once
#ifndef __LibZenBase_Memory_Malloc_h__
#define __LibZenBase_Memory_Malloc_h__

namespace zbMem
{
	//=================================================================================================
	//! @class		MallocAllocator
	//-------------------------------------------------------------------------------------------------
	//! @brief		Basic malloc allocator the simply call the system malloc
	//! @details	Used as a temporary stand-in until more elaborate memory 
	//!				get implemented
	//=================================================================================================
	class MallocAllocator : public zenMem::Allocator
	{
	AWClassDeclare(MallocAllocator, zenMem::Allocator);
	public:
								MallocAllocator(const char* _zName);
		virtual void*			Malloc(size_t _uSize, bool _bIsArray, zenU32 _uAlign);
		virtual	void			Free(void* _pAlloc, void* _pInfoAlloc);
		virtual zenDebugString	GetDescription(){return "Malloc";};
	};
}  

#endif
