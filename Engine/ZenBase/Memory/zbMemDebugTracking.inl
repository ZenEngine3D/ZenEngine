#pragma once

#include <ZenBase/System/zbSysDebugCallstack.h>

namespace zbMem
{
#if ZEN_MEMORY_TRACKING

void* DebugTracking::GetMemory(void* _pMemory)
{
	CheckValidity(_pMemory);
	zU8* pMemory	= reinterpret_cast<zU8*>(_pMemory);
	Header* pHeader	= reinterpret_cast<Header*>(pMemory-sizeof(Header));
	return pHeader;
}

constexpr size_t DebugTracking::GetSizeOverhead()
{
	return sizeof(Header) + sizeof(Footer);
}

size_t DebugTracking::GetSizeNeeded(size_t _SizeWanted)
{	
	return _SizeWanted + GetSizeOverhead();
}

void DebugTracking::CheckValidity(const void* _pMemory)
{	
	if( _pMemory )
	{
		const zU8* pMemory		= reinterpret_cast<const zU8*>(_pMemory);
		const Header* pHeader	= reinterpret_cast<const Header*>(pMemory-sizeof(Header));
		zenAssert(Header::List::IsInList(*pHeader));
		if( pHeader->mAllocFlags.Any(zenMem::keFlag_Protected) == false ) // No footer when Integrity check is active
		{
			static const Footer sReferenceFooter;
			const Footer* pFooter = reinterpret_cast<const Footer*>(pMemory+pHeader->mSizeWanted);
			(void)pFooter;
			zenAssert(pFooter->maFooter == sReferenceFooter.maFooter);		
		}		
	}
}

void* DebugTracking::Malloc(const SAllocInfo& _Allocation, size_t _SizeWanted, zenMem::AllocFlags inAllocFlags)
{	
	const bool bEndOfPage		= inAllocFlags.Any(zenMem::keFlag_Protected);
	zU8* pMemory(_Allocation.pMemory); 
	if( pMemory )
	{		
		pMemory					+= bEndOfPage ? _Allocation.Size - (_SizeWanted + sizeof(Header)) : 0; // Move to end of allocation, to detect memory overflow faster	
		pMemory					= (zU8*)zenMath::RoundDownPow2((size_t)pMemory, zbMem::kAlign);
		Header* pHeader			= new(pMemory)Header();
		pMemory					+= sizeof(Header);
		pHeader->mpCallstack	= nullptr;		
		pHeader->mSizeWanted	= static_cast<zU32>(_SizeWanted);
		pHeader->mSizeUsed		= static_cast<zU32>(_Allocation.Size);
		pHeader->mAllocFlags	= inAllocFlags;
	#if ZEN_MEMORY_TRACKING_DETAILED
		pHeader->mpCallstack	= zbSys::Callstack::GetAddCallstack(2);
	#endif		
		
		// Memory overflow cause page fault when CheckAccess is active, making Footer unnecessary
		if( !bEndOfPage )
		{
			Footer* pFooter		= new(pMemory+_SizeWanted)Footer();
			(void)pFooter;
		}

		mStats.mAllocationCount	+= 1;
		mStats.mSizeWanted		+= pHeader->mSizeWanted;
		mStats.mSizeUsed		+= pHeader->mSizeUsed;
		
		mAllocatedItems.push_front(*pHeader);
	}
	return pMemory;
}

void* DebugTracking::Free(void* _pMemory)
{		
	Header* pHeader				= reinterpret_cast<Header*>(_pMemory)-1;	
	CheckValidity(_pMemory);
	mStats.mAllocationCount		-= 1;
	mStats.mSizeWanted			-= pHeader->mSizeWanted;
	mStats.mSizeUsed			-= pHeader->mSizeUsed;
	mAllocatedItems.remove(*pHeader);
	return pHeader;
}

void* DebugTracking::PreResize(void* _pMemory, size_t _NewSize)
{
	CheckValidity(_pMemory);
	Header* pHeader				= reinterpret_cast<Header*>(_pMemory)-1;
	mStats.mSizeWanted			-= pHeader->mSizeWanted;
	mStats.mSizeUsed			-= pHeader->mSizeUsed;	
	return pHeader;
}

void* DebugTracking::PostResize(void* _pPreResizeAlloc, const SAllocInfo& _Alloc, size_t _SizeWanted)
{
	Header* pHeader				= reinterpret_cast<Header*>(_Alloc.pMemory);
	zU8* pMemory				= _Alloc.pMemory+sizeof(Header);
	if( _pPreResizeAlloc != pMemory )
	{
		Header* pHeaderPrev		= reinterpret_cast<Header*>(_pPreResizeAlloc)-1;
		*pHeader				= *pHeaderPrev;
		pHeader->mLinks->Reset();
		mAllocatedItems.push_front(*pHeader);
	}
	pHeader->mSizeWanted		= static_cast<zU32>(_SizeWanted);
	pHeader->mSizeUsed			= static_cast<zU32>(_Alloc.Size);

	if( pHeader->mAllocFlags.Any(zenMem::keFlag_Protected) == false ) // No footer when Integrity check is active
	{
		Footer* pFooter			= new(pMemory+_SizeWanted)Footer();
		(void)pFooter;
	}	

	mStats.mSizeWanted			+= pHeader->mSizeWanted;
	mStats.mSizeUsed			+= pHeader->mSizeUsed;
	return pMemory;
}

#else

void* DebugTracking::Malloc(SAllocInfo& _Allocation, size_t _SizeWanted, zenMem::AllocFlags inAllocFlags)
{
	return _Allocation.pMemory;
}

void* DebugTracking::Free(void* _pMemory)
{	
	return _pMemory;
}

void* DebugTracking::PreResize(void* _pMemory, size_t _NewSize)
{
	return _pMemory;
}

void DebugTracking::PostResize(void* _pPreResizeAlloc, const SAllocInfo& _Alloc, size_t _SizeWanted)
{
}

void* DebugTracking::GetMemory(void* _pMemory)
{
	return _pMemory;
}

constexpr size_t DebugTracking::GetSizeOverhead()
{
	return 0;
}

size_t DebugTracking::GetSizeNeeded(size_t _SizeWanted)
{
	return _SizeWanted;
}

#endif //ZEN_MEMORY_TRACKING
}  


