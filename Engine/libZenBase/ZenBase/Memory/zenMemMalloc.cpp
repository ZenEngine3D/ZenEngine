#include "libZenBase.h"

namespace zen { namespace zenMem
{

AllocatorMalloc::AllocatorMalloc(const char* _zName)
: zenMem::Allocator(_zName)
{
}

void* AllocatorMalloc::Malloc(size_t _uSize, bool _bIsArray, zenU32 _uAlign)
{	
	size_t uSizeNeeded	= GetAllocSize(_uSize, 0, _uAlign);
	void* pAllocation	= malloc( uSizeNeeded );
	return AddAlloc(_uSize, 0, _uAlign, pAllocation, _bIsArray);

}

void AllocatorMalloc::Free(void* _pAlloc, void* _pInfoAlloc)
{
	ZENAssert(_pAlloc && _pInfoAlloc);
	zbMem::AllocHeader* pInfoAlloc = static_cast<zbMem::AllocHeader*>(_pInfoAlloc);
	RemAlloc(pInfoAlloc);
	free( (void*)((zenPointer)_pAlloc - pInfoAlloc->muOffset) );
}

}} //namespace zen { namespace zenMem