#include "libZenBase.h"

namespace zbMem
{
#define kh32AllocatorStamp	zenHash32("AllocatorValid")	//Note Using define and not constant, since we must be sure value is valid before main() is reached (for global heap allocs)
#define kh64AllocationStamp zenHash64("AllocationEnd")

static zenMem::Allocator* GDefault = NULL;

AllocHeader* GetHeader( void* _pAlloc, bool _bIsArray )
{
	AllocHeader* pHeader = ((AllocHeader*)( _pAlloc ))-1;
	if( pHeader->IsValid() )
	{
		AWCriticalMsg( pHeader->mpAllocator->GetValidityStamp()				== kh32AllocatorStamp, "Data corruption occurred, invalid memory allocator pointer" );	
		AWAssertMsg(*(zenHash64*)((zenPointer)_pAlloc+pHeader->muWantedSize)== kh64AllocationStamp, "Data corruption occured, footer memory was trashed. Buffer overrun?" );	
		AWAssertMsg( pHeader->IsArray() == _bIsArray, "Free 'new' with 'delete' and 'new[]' with 'delete[]', do not mismatch." );
		return pHeader;
	}
	return NULL;
}

zenMem::Allocator& GetDefault()
{
	static zenMem::AllocatorMalloc sMalloc("DefaultMalloc");
	if( GDefault == NULL )
		GDefault = &sMalloc; //! @todo: replace this with scope override
	//static zbMem::Override			sDefaultMemAlloc( &sMalloc );
	return *GDefault;
}

void AllocHeader::Set(zenMem::Allocator* _pAllocator, zenU32 _uAllocOffset, size_t _uAllocSize, bool _bIsArray)
{
	mpAllocator		= _pAllocator;
	mhStamp			= zenHash32("ValidAlloc");
	muOffset		= _uAllocOffset;
	mbIsArray		= _bIsArray;
	muWantedSize	= _uAllocSize;	
}


/*
AllocatorBase::AllocatorBase(zenDebugString _zName)
: mh32ValidStamp( kh32AllocatorStamp )
, mzAllocatorName(_zName)	
, muTotalAllocSize(0)
, muTotalAllocCount(0)
{
}

AllocatorBase::~AllocatorBase()
{
	AWAssertMsg( muTotalAllocCount==0, "Trying to delete _ memory manager that still has elements allocated");
}

void AllocatorBase::DebugPrint()
{
#if AW_MEMORYDEBUG
	CLog::Log( CLog::keLog_DebugInfo, awconst::kzLineA60 );
	CLog::Log( CLog::keLog_DebugInfo, "Memory" );
	CLog::Log( CLog::keLog_DebugInfo, awconst::kzLineA60 );
	zenList2xNode* pAlloc = mlstAllocations.GetHead();
	while( pAlloc != mlstAllocations.GetInvalid() )
	{
		pAlloc = pAlloc->LstNext();
	}
	
	//! @TODO: Print informations _bout _llocations
// 	char		zSpaces[kuTreeMaxDepth*kuSpacePerLevel+1];
// 	zenS8			sSlotIndexPrev[ sizeof(((Iterator*)NULL)->mpSlotIndex) ];	//Get size of non-instantiated member _rray
// 	memset(zSpaces, ' ', sizeof(zSpaces));
// 	memset(sSlotIndexPrev, 0xFF, sizeof(sSlotIndexPrev) );
// 	for(Iterator it=GetFirst(); it.IsValid(); ++it)
// 	{
// 		zenS8 sSharedParentDepth(0);
// 		zenS8 sCurrentDepth(0);
// 		while( sSlotIndexPrev[sSharedParentDepth] == it.mpSlotIndex[sSharedParentDepth] )
// 			++sSharedParentDepth;
// 		zenMem::Copy(sSlotIndexPrev, it.mpSlotIndex, sizeof(sSlotIndexPrev) );
// 
// 	 zenHash32 h32Key = it.GetKey();
// 		zSpaces[kuSpacePerLevel*sSharedParentDepth] = 0; 
// 		CLog::Printf(CLog::keLog_Game, "\n%s", zSpaces);
// 		zSpaces[kuSpacePerLevel*sSharedParentDepth] = ' '; 
// 		sCurrentDepth = sSharedParentDepth;
// 		while( sCurrentDepth != it.msDepth )
// 			CLog::Printf(CLog::keLog_Game, "<%02i>----------", (h32Key>>(kuIndexSize*sCurrentDepth++))&kuIndexMask);
// 		CLog::Printf(CLog::keLog_Game, "<%02i> %08X", (h32Key>>(kuIndexSize*sCurrentDepth))&kuIndexMask, h32Key);
// 	}
#endif

}



size_t AllocatorBase::GetAllocSize(size_t _uWantedSize, size_t _uExtraSize, zenU32 _uAlign )
{
	AWAssertMsg( zenMath::IsPower2(_uAlign) && _uAlign>=sizeof(void*), "Alignement must be a power of 2 and greater equal than pointer data size" );	
	return _uWantedSize + _uExtraSize + _uAlign + sizeof(AllocHeader) + sizeof(kh64AllocationStamp);
}

void* AllocatorBase::AddAlloc( size_t _uWantedSize, size_t _uExtraSize, zenU32 _uAlign, void* _pAllocation, const bool _bIsArray )
{
	size_t uAlignMask								= size_t(_uAlign)-1;
	zenU8* pAllocAligned								= static_cast<zenU8*>(_pAllocation) + sizeof(AllocHeader) + _uExtraSize;
	size_t uAlignPadding							= (static_cast<size_t>(_uAlign) - ((size_t)pAllocAligned & uAlignMask)) & uAlignMask; 
	pAllocAligned									+= uAlignPadding;
	AllocHeader* pHeader									= ((AllocHeader*)( pAllocAligned ))-1;	
	*(zenHash64*)(pAllocAligned+_uWantedSize)	= kh64AllocationStamp;
	pHeader->Set(this, (zenU32)((zenPointer)pAllocAligned-(zenPointer)_pAllocation), _uWantedSize, _bIsArray);
	muTotalAllocSize								+= _uWantedSize;
	muTotalAllocCount								+= 1;
#if AW_MEMORYDEBUG
	pHeader->LstReset();
	mlstAllocations.AddHead( pHeader );
#endif
	return pAllocAligned;
}

void AllocatorBase::RemAlloc(AllocHeader* _pAlloc)
{	
	muTotalAllocSize	-= _pAlloc->muWantedSize;
	muTotalAllocCount	-= 1;
#if AW_MEMORYDEBUG
	_pAlloc->LstRemove();
#endif
}
*/
}  

