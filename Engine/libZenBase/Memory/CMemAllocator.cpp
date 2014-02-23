#include "libZenBase.h"

namespace CMem
{
#define kh32AllocatorStamp	awHash32("AllocatorValid")	//Note Using define and not constant, since we must be sure value is valid before main() is reached (for global heap allocs)
#define kh64AllocationStamp awHash64("AllocationEnd")

Allocator* Allocator::Default = NULL;

Allocator::Allocator(awDebugString _zName)
: mh32ValidStamp( kh32AllocatorStamp )
, mzAllocatorName(_zName)	
, muTotalAllocSize(0)
, muTotalAllocCount(0)
{
}

Allocator::~Allocator()
{
	AWAssertMsg( muTotalAllocCount==0, "Trying to delete _ memory manager that still has elements allocated");
}

void Allocator::DebugPrint()
{
#if AW_MEMORYDEBUG
	CLog::Log( CLog::keLog_DebugInfo, awconst::kzLineA60 );
	CLog::Log( CLog::keLog_DebugInfo, "Memory" );
	CLog::Log( CLog::keLog_DebugInfo, awconst::kzLineA60 );
	awList2xNode* pAlloc = mlstAllocations.GetHead();
	while( pAlloc != mlstAllocations.GetInvalid() )
	{
	}
	/*
	//! @TODO: Print informations _bout _llocations
	char		zSpaces[kuTreeMaxDepth*kuSpacePerLevel+1];
	awS8			sSlotIndexPrev[ sizeof(((Iterator*)NULL)->mpSlotIndex) ];	//Get size of non-instantiated member _rray
	memset(zSpaces, ' ', sizeof(zSpaces));
	memset(sSlotIndexPrev, 0xFF, sizeof(sSlotIndexPrev) );
	for(Iterator it=GetFirst(); it.IsValid(); ++it)
	{
		awS8 sSharedParentDepth(0);
		awS8 sCurrentDepth(0);
		while( sSlotIndexPrev[sSharedParentDepth] == it.mpSlotIndex[sSharedParentDepth] )
			++sSharedParentDepth;
		zenMem::Copy(sSlotIndexPrev, it.mpSlotIndex, sizeof(sSlotIndexPrev) );

	 awHash32 h32Key = it.GetKey();
		zSpaces[kuSpacePerLevel*sSharedParentDepth] = 0; 
		CLog::Printf(CLog::keLog_Game, "\n%s", zSpaces);
		zSpaces[kuSpacePerLevel*sSharedParentDepth] = ' '; 
		sCurrentDepth = sSharedParentDepth;
		while( sCurrentDepth != it.msDepth )
			CLog::Printf(CLog::keLog_Game, "<%02i>----------", (h32Key>>(kuIndexSize*sCurrentDepth++))&kuIndexMask);
		CLog::Printf(CLog::keLog_Game, "<%02i> %08X", (h32Key>>(kuIndexSize*sCurrentDepth))&kuIndexMask, h32Key);
	}*/
#endif

}


bool Allocator::Header::IsArray()
{
	return mbIsArray;
}

void Allocator::Header::Set(Allocator* _pAllocator, awU32 _uAllocOffset, size_t _uAllocSize, bool _bIsArray)
{
	mpAllocator		= _pAllocator;
	mhStamp			= awHash32("ValidAlloc");
	muOffset		= _uAllocOffset;
	mbIsArray		= _bIsArray;
	muWantedSize	= _uAllocSize;	
}

size_t Allocator::GetAllocSize(size_t _uWantedSize, size_t _uExtraSize, awU32 _uAlign )
{
	AWAssertMsg( zenMath::IsPower2(_uAlign) && _uAlign>=sizeof(void*), "Alignement must be a power of 2 and greater equal than pointer data size" );	
	return _uWantedSize + _uExtraSize + _uAlign + sizeof(Header) + sizeof(kh64AllocationStamp);
}

void* Allocator::AddAlloc( size_t _uWantedSize, size_t _uExtraSize, awU32 _uAlign, void* _pAllocation, const bool _bIsArray )
{
	size_t uAlignMask								= size_t(_uAlign)-1;
	awU8* pAllocAligned								= static_cast<awU8*>(_pAllocation) + sizeof(Header) + _uExtraSize;
	size_t uAlignPadding							= (static_cast<size_t>(_uAlign) - ((size_t)pAllocAligned & uAlignMask)) & uAlignMask; 
	pAllocAligned									+= uAlignPadding;
	Header* pHeader									= ((Header*)( pAllocAligned ))-1;	
	*(awHash64*)(pAllocAligned+_uWantedSize)	= kh64AllocationStamp;
	pHeader->Set(this, (awU32)((awPointer)pAllocAligned-(awPointer)_pAllocation), _uWantedSize, _bIsArray);
	muTotalAllocSize								+= _uWantedSize;
	muTotalAllocCount								+= 1;
#if AW_MEMORYDEBUG
	pHeader->LstReset();
	mlstAllocations.AddHead( pHeader );
#endif
	return pAllocAligned;
}

void Allocator::RemAlloc(Header* _pAlloc)
{	
	muTotalAllocSize	-= _pAlloc->muWantedSize;
	muTotalAllocCount	-= 1;
#if AW_MEMORYDEBUG
	apAlloc->LstRemove();
#endif
}

Allocator::Header* Allocator::GetHeader( void* _pAlloc, bool _bIsArray )
{
	Allocator::Header* pHeader = ((Allocator::Header*)( _pAlloc ))-1;
	if( pHeader->IsValid() )
	{
		AWCriticalMsg( pHeader->mpAllocator->mh32ValidStamp				== kh32AllocatorStamp, "Data corruption occurred, invalid memory allocator pointer" );	
		AWAssertMsg(*(awHash64*)((awPointer)_pAlloc+pHeader->muWantedSize)	== kh64AllocationStamp, "Data corruption occured, footer memory was trashed. Buffer overrun?" );	
		AWAssertMsg( pHeader->IsArray() == _bIsArray, "Free 'new' with 'delete' and 'new[]' with 'delete[]', do not mismatch." );
		return pHeader;
	}
	return NULL;
}

Allocator& Allocator::GetDefault()
{
	static CMem::MallocAllocator sMalloc("DefaultMalloc");
	static CMem::Override sDefaultMemAlloc( &sMalloc );
	return *Default;
}

}  

