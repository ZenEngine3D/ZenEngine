#include "zxExternal.h"
#include <malloc.h>
#include <assert.h>

//!================================================================================================
//! @details 	Default EASTL allocator 
//!================================================================================================
#ifdef ZEN_THIRDPARTY_EASTL

#if EASTL_NAME_ENABLED
	#define SetName(NAME)	mpName = (NAME)
	#define GetName			mpName
#else
	#define SetName(NAME)	
	#define GetName			EASTL_ALLOCATOR_DEFAULT_NAME
#endif

namespace eastl
{

allocator::allocator(const char* EASTL_NAME(pName))
{
	SetName(pName ? pName : EASTL_ALLOCATOR_DEFAULT_NAME);
}

allocator::allocator(const allocator& EASTL_NAME(alloc))
{
	SetName(alloc.mpName);
}

allocator::allocator(const allocator&, const char* EASTL_NAME(pName))
{
	SetName(pName ? pName : EASTL_ALLOCATOR_DEFAULT_NAME);
}

allocator& allocator::operator=(const allocator& EASTL_NAME(alloc))
{
	SetName(alloc.mpName);
	return *this;
}

const char* allocator::get_name() const
{
	return GetName;
}

void allocator::set_name(const char* EASTL_NAME(pName))
{
	SetName(pName);
}

void* allocator::allocate(size_t n, int flags)
{
	return zbMem::Allocate(n, 1, zenMem::AllocFlags());
}

void* allocator::allocate(size_t n, size_t alignment, size_t offset, int flags)
{
	if( alignment > zbMem::kAlign || offset != 0)
	{
		zenAssert(0); //Currently Unsupported custom alignment
	}
	return zbMem::Allocate(n, 1, zenMem::AllocFlags());
#if 0
	// We currently have no support for implementing flags when 
	// using the C runtime library operator new function. The user 
	// can use SetDefaultAllocator to override the default allocator.
	EA_UNUSED(offset); EA_UNUSED(flags);

	size_t adjustedAlignment = (alignment > EA_PLATFORM_PTR_SIZE) ? alignment : EA_PLATFORM_PTR_SIZE;

	void* p = new char[n + adjustedAlignment + EA_PLATFORM_PTR_SIZE];
	void* pPlusPointerSize = (void*)((uintptr_t)p + EA_PLATFORM_PTR_SIZE);
	void* pAligned = (void*)(((uintptr_t)pPlusPointerSize + adjustedAlignment - 1) & ~(adjustedAlignment - 1));

	void** pStoredPtr = (void**)pAligned - 1;
	EASTL_ASSERT(pStoredPtr >= p);
	*(pStoredPtr) = p;

	EASTL_ASSERT(((size_t)pAligned & ~(alignment - 1)) == (size_t)pAligned);

	return pAligned;
#endif
}

void allocator::deallocate(void* p, size_t)
{
	zbMem::Deallocate(p);
}

bool operator==(const allocator&, const allocator&)
{
	return true; // All allocators are considered equal, as they merely use global new/delete.
}


bool operator!=(const allocator&, const allocator&)
{
	return false; // All allocators are considered equal, as they merely use global new/delete.
}

}

#endif
