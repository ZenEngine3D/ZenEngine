#pragma once
#ifndef __zenBase_Memory_Malloc_h__
#define __zenBase_Memory_Malloc_h__

namespace zen { namespace zenMem
{
	//=================================================================================================
	//! @class		AllocatorMalloc
	//-------------------------------------------------------------------------------------------------
	//! @brief		Basic malloc allocator the simply call the system malloc
	//! @details	Used as a temporary stand-in until more elaborate memory 
	//!				get implemented
	//=================================================================================================
	class AllocatorMalloc : public zenMem::Allocator
	{
	ZENClassDeclare(AllocatorMalloc, zenMem::Allocator);
	public:
								AllocatorMalloc(const char* _zName);
		virtual void*			Malloc(size_t _uSize, bool _bIsArray, zenU32 _uAlign);
		virtual	void			Free(void* _pAlloc, void* _pInfoAlloc);
		virtual zenDebugString	GetDescription(){return "Malloc";};
	};
}} //namespace zen { namespace zenMem

#endif
