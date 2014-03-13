#include "libZenBase.h"

namespace zen { namespace zenMem
{

zAllocatorMalloc::zAllocatorMalloc(const char* _zName)
: zenMem::zAllocator(_zName)
{
}

void* zAllocatorMalloc::Malloc(size_t _uSize, bool _bIsArray, zU32 _uAlign)
{	
	size_t uSizeNeeded	= GetAllocSize(_uSize, 0, _uAlign);
	void* pAllocation	= malloc( uSizeNeeded );
	return AddAlloc(_uSize, 0, _uAlign, pAllocation, _bIsArray);

}

void zAllocatorMalloc::Free(void* _pAlloc, void* _pInfoAlloc)
{
	ZENAssert(_pAlloc && _pInfoAlloc);
	zbMem::AllocHeader* pInfoAlloc = static_cast<zbMem::AllocHeader*>(_pInfoAlloc);
	RemAlloc(pInfoAlloc);
	free( (void*)((zU8*)_pAlloc - pInfoAlloc->muOffset) );
}

}} //namespace zen { namespace zenMem