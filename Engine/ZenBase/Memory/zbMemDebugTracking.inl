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
		if( !pHeader->mIsCheckAccess )
		{
			static Footer sReferenceFooter;
			const Footer* pFooter = reinterpret_cast<const Footer*>(pMemory+pHeader->mSizeWanted);
			(void)pFooter;
			zenAssert(pFooter->maFooter == sReferenceFooter.maFooter);		
		}		
	}
}

void* DebugTracking::Malloc(const SAllocInfo& _Allocation, size_t _SizeWanted, size_t _SizeMax, const char* _Filename, int _LineNumber, bool _IsArrayNew, bool _IsCheckAccess)
{	
	zU8* pMemory(_Allocation.pMemory); 
	if( pMemory )
	{		
		pMemory					+= _IsCheckAccess ? _Allocation.Size - (_SizeWanted + sizeof(Header)) : 0; // Move to end of allocation, to detect memory overflow faster	
		Header* pHeader			= new(pMemory)Header();
		pMemory					+= sizeof(Header);
		pHeader->mFileName		= _Filename;
		pHeader->mFileLine		= _LineNumber;
		pHeader->mpCallstack	= nullptr;		
		pHeader->mSizeWanted	= static_cast<zU32>(_SizeWanted);
		pHeader->mSizeUsed		= static_cast<zU32>(_Allocation.Size);
		pHeader->mIsArrayNew	= _IsArrayNew ? 1 : 0;	
		pHeader->mIsCheckAccess	= _IsCheckAccess ? 1 : 0;
	#if ZEN_MEMORY_TRACKING_DETAILED
		pHeader->mpCallstack	= zbSys::Callstack::GetAddCallstack(2);
	#endif		
		
		// Memory overflow cause page fault when CheckAccess is active, making Footer unnecessary
		if( !_IsCheckAccess )
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

void* DebugTracking::Free(void* _pMemory, bool _IsArrayDel)
{		
	zU8* pMemory				= reinterpret_cast<zU8*>(_pMemory);
	Header* pHeader				= reinterpret_cast<Header*>(pMemory-sizeof(Header));
	
	CheckValidity(_pMemory);
	zenAssert((pHeader->mIsArrayNew != 0) == _IsArrayDel);

	mStats.mAllocationCount		-= 1;
	mStats.mSizeWanted			-= pHeader->mSizeWanted;
	mStats.mSizeUsed			-= pHeader->mSizeUsed;
	mAllocatedItems.remove(*pHeader);
	return pHeader;
}

void* DebugTracking::PreResize(void* _pMemory, size_t _NewSize)
{
	CheckValidity(_pMemory);
	zU8* pMemory				= reinterpret_cast<zU8*>(_pMemory);
	Header* pHeader				= reinterpret_cast<Header*>(pMemory-sizeof(Header));

	mStats.mSizeWanted			-= pHeader->mSizeWanted;
	mStats.mSizeUsed			-= pHeader->mSizeUsed;	
	return pHeader;
}

void* DebugTracking::PostResize(const SAllocInfo& Alloc, size_t _NewSize)
{
	zU8* pMemory				= Alloc.pMemory+sizeof(Header);
	Header* pHeader				= reinterpret_cast<Header*>(Alloc.pMemory);
	Footer* pFooter				= new(pMemory+_NewSize)Footer();
	(void)pFooter;
	
	pHeader->mSizeWanted		= static_cast<zU32>(_NewSize);
	pHeader->mSizeUsed			= static_cast<zU32>(GetSizeNeeded(_NewSize));	
	mStats.mSizeWanted			+= pHeader->mSizeWanted;
	mStats.mSizeUsed			+= pHeader->mSizeUsed;
	return pMemory;
}

#else

void* DebugTracking::Malloc(SAllocInfo& _Allocation, size_t _SizeWanted, size_t _SizeMax, const char* _Filename, int _LineNumber, bool _IsArrayNew, bool _IsTrackingBadAccess)
{
	return _Allocation.pMemory;
}

void* DebugTracking::Free(void* _pMemory, bool _IsArrayDel)
{	
	return _pMemory;
}

void* DebugTracking::PreResize(void* _pMemory, size_t _NewSize)
{
	return _pMemory;
}

void DebugTracking::PostResize(void* _pMemory, size_t _NewSize)
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


