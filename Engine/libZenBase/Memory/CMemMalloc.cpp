#include "libZenBase.h"

namespace CMem
{

MallocAllocator::MallocAllocator(const char* _zName)
: Allocator(_zName)
{
}

void* MallocAllocator::Malloc(size_t _uSize, bool _bIsArray, zenU32 _uAlign)
{	
	size_t uSizeNeeded	= GetAllocSize(_uSize, 0, _uAlign);
	void* pAllocation	= malloc( uSizeNeeded );
	return AddAlloc(_uSize, 0, _uAlign, pAllocation, _bIsArray);

}

void MallocAllocator::Free(void* _pAlloc, Header* _pInfoAlloc)
{
	AWAssert(_pAlloc && _pInfoAlloc);
	RemAlloc(_pInfoAlloc);
	free( (void*)((zenPointer)_pAlloc - _pInfoAlloc->muOffset) );
}

}  