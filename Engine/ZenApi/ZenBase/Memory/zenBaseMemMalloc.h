#pragma once
#ifndef __zenApi_Base_Memory_Malloc_h__
#define __zenApi_Base_Memory_Malloc_h__

namespace zen { namespace zenMem
{
	//=================================================================================================
	//! @class		zAllocatorMalloc
	//-------------------------------------------------------------------------------------------------
	//! @brief		Basic malloc allocator the simply call the system malloc
	//! @details	Used as a temporary stand-in until more elaborate memory 
	//!				get implemented
	//=================================================================================================
	class zAllocatorMalloc : public zenMem::zAllocator
	{
	ZENClassDeclare(zAllocatorMalloc, zenMem::zAllocator);
	public:
								zAllocatorMalloc(const char* _zName);
		virtual void*			Malloc(size_t _uSize, bool _bIsArray, zU32 _uAlign);
		virtual	void			Free(void* _pAlloc, void* _pInfoAlloc);
		virtual zDebugString	GetDescription(){return "Malloc";};
	};
}} //namespace zen { namespace zenMem

#endif
