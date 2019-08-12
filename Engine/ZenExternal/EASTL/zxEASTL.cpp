#include "zxExternal.h"
#include <malloc.h>
#include <assert.h>

//!================================================================================================
//! @details 	Default EASTL behavior expect user to define those 2 'new' allocator override
//!				Went simple, and used default OS behavior
//! @todo 0 Clean use zen allocators
//!================================================================================================
#ifdef ZEN_THIRDPARTY_EASTL

//The flags and debugFlags arguments correspond to PPMalloc/RenderWare GeneralAllocator/GeneralAllocatorDebug Malloc equivalents.
void* operator new[](size_t inSize, const char* inName, int inFlags, unsigned inDebugFlags, const char* inFile, int inLine)
{
	(void)inName;		// Ignore debugging informations
	(void)inFlags;
	(void)inDebugFlags;	
	return zbMem::Allocate(inSize, 1, zenMem::AllocFlags());
}

void* operator new[](size_t inSize, size_t inAlign, size_t inAlignOffset, const char* inName, int inFlags, unsigned inDebugFlags, const char* inFile, int inLine)
{
	(void)inName;		// Ignore debugging informations
	(void)inFlags;
	(void)inDebugFlags;
	//! @todo 0 Manage align
	return zbMem::Allocate(inSize, 1, zenMem::AllocFlags());
}

#endif
