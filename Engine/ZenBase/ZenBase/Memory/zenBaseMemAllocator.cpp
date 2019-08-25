#include "zbBase.h"

namespace zbMem
{

void UpdateAllocationFlags(size_t inSize, size_t inItemCount, zenMem::AllocFlags& inAllocFlags)
{
#if ZEN_MEMORY_SUPPORT_CHECKACCESS
	// Can implement additional logic based on allocation size, etc...
	// ...	
//inAllocFlags += zenMem::keFlag_Protected;;//SF
#else
	inAllocFlags -= zenMem::keFlag_Protected;
#endif 

#if ZEN_MEMORY_FORCE_NATIVE_MALLOC
	inAllocFlags += zenMem::keFlag_NativeMalloc;	
#endif

	if( inAllocFlags.Any(zenMem::keFlag_NativeMalloc) )
		inAllocFlags -= zenMem::keFlag_Protected;
}

Allocator& GetMemoryAllocator()
{
	static zbMem::Allocator MemoryAllocator;
	return MemoryAllocator;
}

void* Allocate(size_t inSize, size_t inItemCount, zenMem::AllocFlags inAllocFlags)
{
	UpdateAllocationFlags(inSize, inItemCount, inAllocFlags);
	return GetMemoryAllocator().Malloc(inSize, inItemCount, inAllocFlags);
}
	
void* ResizeMemory(void* _pMemory, size_t _NewSize, size_t inItemCount)
{
	return GetMemoryAllocator().Resize(_pMemory, _NewSize, inItemCount);
}

void Deallocate(void* _pMemory)
{
	GetMemoryAllocator().Free(_pMemory);
}

size_t GetItemCount(void* _pMemory)
{
	return GetMemoryAllocator().GetItemCount(_pMemory);
}

}