#include "zbBase.h"

namespace zbMem
{
#define kh32AllocatorStamp	zHash32("AllocatorValid")	//Note Using define and not constant, since we must be sure value is valid before main() is reached (for global heap allocs)
#define kh64AllocationStamp zHash64("AllocationEnd")

AllocHeader* GetHeader( void* _pAlloc, bool _bIsArray )
{
	AllocHeader* pHeader = ((AllocHeader*)( _pAlloc ))-1;
	if( pHeader->IsValid() )
	{
		ZENCriticalMsg( pHeader->mpAllocator->GetValidityStamp()		== kh32AllocatorStamp, "Data corruption occurred, invalid memory allocator pointer" );	
		ZENAssertMsg(*(zHash64*)((zU8*)_pAlloc+pHeader->muWantedSize)	== kh64AllocationStamp, "Data corruption occured, footer memory was trashed. Buffer overrun?" );	
		ZENAssertMsg( pHeader->IsArray() == _bIsArray, "Free 'new' with 'delete' and 'new[]' with 'delete[]', do not mismatch." );
		return pHeader;
	}
	return nullptr;
}

void AllocHeader::Set(zenMem::zAllocator* _pAllocator, zU32 _uAllocOffset, size_t _uAllocSize, bool _bIsArray)
{
	mpAllocator		= _pAllocator;
	mhStamp			= zHash32("ValidAlloc");
	muOffset		= _uAllocOffset;
	mbIsArray		= _bIsArray;
	muWantedSize	= _uAllocSize;	
}



}  

