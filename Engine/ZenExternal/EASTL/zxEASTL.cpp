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
	(void)inName;	// Ignore debugging informations
	(void)inFlags;
	(void)inDebugFlags;
	(void)inFile;
	(void)inLine;
	return malloc(inSize);
}

void* operator new[](size_t inSize, size_t inAlign, size_t inAlignOffset, const char* inName, int inFlags, unsigned inDebugFlags, const char* inFile, int inLine)
{
	(void)inName;		// Ignore debugging informations
	(void)inFlags;
	(void)inDebugFlags;
	(void)inFile;
	(void)inLine;
	const size_t AlignMask	= inAlign-1;
	assert( (inAlign & AlignMask) == 0 );	//Makes sure alignment is a power of 2
	size_t MemAdr			= (size_t)malloc(inSize+inAlign);
	return (void*)(MemAdr + ((MemAdr+inAlignOffset) & AlignMask)-inAlignOffset);
}

#endif
