#include "libZenBase.h"

#define kh32AllocatorStamp	zHash32("AllocatorValid")	//Note Using define and not constant, since we must be sure value is valid before main() is reached (for global heap allocs)
#define kh64AllocationStamp zHash64("AllocationEnd")

namespace zen { namespace zenMem
{

ZENInline zList2x& GetAllocatorScopeList()
{
	static zList2x sOverideList;
	return sOverideList;
}

ScopedAllocator::ScopedAllocator(zAllocator* _pAllocator)
: mpAllocator(_pAllocator)
{
	ZENAssert(mpAllocator);
	zList2x* testList = &GetAllocatorScopeList();
	GetAllocatorScopeList().AddTail(this);
}
ScopedAllocator::~ScopedAllocator()
{
	LstRemove();
	ZENAssert(!GetAllocatorScopeList().IsEmpty());	//! Should be impossible with global 'GAllocatorOveride'
}		

zAllocator& ScopedAllocator::GetActive()
{
	return *static_cast<ScopedAllocator*>(GetAllocatorScopeList().GetTail())->mpAllocator;
}

zAllocator& zAllocator::GetDefault()
{
	static zAllocatorMalloc sAllocatorMalloc("DefaultMalloc");
	static ScopedAllocator sAllocatorOveride(&sAllocatorMalloc);
	return ScopedAllocator::GetActive();
}

zAllocator::zAllocator(zDebugString _zName)
: mh32ValidStamp( kh32AllocatorStamp )
, mzAllocatorName(_zName)	
, muTotalAllocSize(0)
, muTotalAllocCount(0)
{
}

zAllocator::~zAllocator()
{
	ZENAssertMsg( muTotalAllocCount==0, "Trying to delete _ memory manager that still has elements allocated");
}
/*
void zAllocator::DebugPrint()
{
#if AW_MEMORYDEBUG
	CLog::Log( CLog::keLog_DebugInfo, zenConst::kzLineA60 );
	CLog::Log( CLog::keLog_DebugInfo, "Memory" );
	CLog::Log( CLog::keLog_DebugInfo, zenConst::kzLineA60 );
	zList2xNode* pAlloc = mlstAllocations.GetHead();
	while( pAlloc != mlstAllocations.GetInvalid() )
	{
		pAlloc = pAlloc->LstNext();
	}
	
	! @TODO: Print informations _bout _llocations
		char		zSpaces[kuTreeMaxDepth*kuSpacePerLevel+1];
		zS8			sSlotIndexPrev[ sizeof(((Iterator*)NULL)->mpSlotIndex) ];	//Get size of non-instantiated member _rray
		memset(zSpaces, ' ', sizeof(zSpaces));
		memset(sSlotIndexPrev, 0xFF, sizeof(sSlotIndexPrev) );
		for(Iterator it=GetFirst(); it.IsValid(); ++it)
		{
			zS8 sSharedParentDepth(0);
			zS8 sCurrentDepth(0);
			while( sSlotIndexPrev[sSharedParentDepth] == it.mpSlotIndex[sSharedParentDepth] )
				++sSharedParentDepth;
			zenMem::Copy(sSlotIndexPrev, it.mpSlotIndex, sizeof(sSlotIndexPrev) );
	
		 zHash32 h32Key = it.GetKey();
			zSpaces[kuSpacePerLevel*sSharedParentDepth] = 0; 
			CLog::Printf(CLog::keLog_Game, "\n%s", zSpaces);
			zSpaces[kuSpacePerLevel*sSharedParentDepth] = ' '; 
			sCurrentDepth = sSharedParentDepth;
			while( sCurrentDepth != it.msDepth )
				CLog::Printf(CLog::keLog_Game, "<%02i>----------", (h32Key>>(kuIndexSize*sCurrentDepth++))&kuIndexMask);
			CLog::Printf(CLog::keLog_Game, "<%02i> %08X", (h32Key>>(kuIndexSize*sCurrentDepth))&kuIndexMask, h32Key);
		}
#endif

}
*/
size_t zAllocator::GetAllocSize(size_t _uWantedSize, size_t _uExtraSize, zU32 _uAlign )
{
	ZENAssertMsg( zenMath::IsPower2(_uAlign) && _uAlign>=sizeof(void*), "Alignement must be a power of 2 and greater equal than pointer data size" );	
	return _uWantedSize + _uExtraSize + _uAlign + sizeof(zbMem::AllocHeader) + sizeof(kh64AllocationStamp);
}

void* zAllocator::AddAlloc( size_t _uWantedSize, size_t _uExtraSize, zU32 _uAlign, void* _pAllocation, const bool _bIsArray )
{
	size_t uAlignMask						= size_t(_uAlign)-1;
	zU8* pAllocAligned						= static_cast<zU8*>(_pAllocation) + sizeof(zbMem::AllocHeader) + _uExtraSize;
	size_t uAlignPadding					= (static_cast<size_t>(_uAlign) - ((size_t)pAllocAligned & uAlignMask)) & uAlignMask; 
	pAllocAligned							+= uAlignPadding;
	zbMem::AllocHeader* pHeader				= ((zbMem::AllocHeader*)( pAllocAligned ))-1;	
	*(zHash64*)(pAllocAligned+_uWantedSize)	= kh64AllocationStamp;
	pHeader->Set(this, (zU32)((zU8*)pAllocAligned-(zU8*)_pAllocation), _uWantedSize, _bIsArray);
	muTotalAllocSize						+= _uWantedSize;
	muTotalAllocCount						+= 1;
#if AW_MEMORYDEBUG
	pHeader->LstReset();
	mlstAllocations.AddHead( pHeader );
#endif
	return pAllocAligned;
}

void zAllocator::RemAlloc(void* _pAlloc)
{	
	zbMem::AllocHeader* pAlloc	= static_cast<zbMem::AllocHeader*>(_pAlloc);
	muTotalAllocSize			-= pAlloc->muWantedSize;
	muTotalAllocCount			-= 1;
#if AW_MEMORYDEBUG
	pAlloc->LstRemove();
#endif
}

}} //namespace zen { namespace mem  

void* zMalloc(size_t _uSize_, zU32 uAlign)
{
	return zenMem::zAllocator::GetDefault().Malloc(_uSize_, false, uAlign);
}

void* zMalloc(zenMem::zAllocator* _pAllocator, size_t _uSize_, zU32 uAlign)
{
	_pAllocator = _pAllocator ? _pAllocator : &zenMem::zAllocator::GetDefault();
	return _pAllocator->Malloc(_uSize_, false, uAlign);
}

void* operator new(size_t _uSize, zenMem::zAllocator* _pAllocator)
{
	_pAllocator = _pAllocator ? _pAllocator : &zenMem::zAllocator::GetDefault();
	return _pAllocator->Malloc(_uSize, false, zenDefaultAlign);
}

void* operator new[](size_t _uSize, zenMem::zAllocator* _pAllocator )
{
	_pAllocator = _pAllocator ? _pAllocator : &zenMem::zAllocator::GetDefault();
	return _pAllocator->Malloc(_uSize, true, zenDefaultAlign);
}

void* operator new(size_t _uSize, zenMem::zAllocator* _pAllocator, zUInt _uAlign )
{
	_pAllocator = _pAllocator ? _pAllocator : &zenMem::zAllocator::GetDefault();
	return _pAllocator->Malloc(_uSize, false, _uAlign);
}

void* operator new[](size_t _uSize, zenMem::zAllocator* _pAllocator, zUInt _uAlign )
{
	_pAllocator = _pAllocator ? _pAllocator : &zenMem::zAllocator::GetDefault();
	return _pAllocator->Malloc(_uSize, true, _uAlign);
}

void operator delete(void* _pAlloc)
{
	if( _pAlloc )
	{
		zbMem::AllocHeader* pHeaderInfo = zbMem::GetHeader(_pAlloc, false);
		if( pHeaderInfo)
			pHeaderInfo->mpAllocator->Free(_pAlloc, pHeaderInfo);	
		else 
			free(_pAlloc);
	}	
}

void operator delete[](void* _pAlloc)
{
	if( _pAlloc )
	{
		zbMem::AllocHeader* pHeaderInfo = zbMem::GetHeader(_pAlloc, true);
		if( pHeaderInfo)
			pHeaderInfo->mpAllocator->Free(_pAlloc, pHeaderInfo);	
		else 
			free(_pAlloc);
	}	
}

//=================================================================================================
//! @brief		If exception occurs in constructor, this function is called for new
//! @details	A function matching the 'new' signature is called when there's an exception in the 
//!				constructor of an object, instead of calling the regular delete overload.
//!				This function contains all the common code for 'non array new' matching 'delete'
//=================================================================================================
void DeleteCatchException(void* _pAlloc, zenMem::zAllocator* _pAllocator )
{
	if( _pAlloc )
	{
		zbMem::AllocHeader* pHeaderInfo = zbMem::GetHeader(_pAlloc, false);
		if( pHeaderInfo)
			pHeaderInfo->mpAllocator->Free(_pAlloc, pHeaderInfo);	
	}
}

//=================================================================================================
//! @brief		If exception occurs in constructor, this function is called for new
//! @details	A function matching the 'new' signature is called when there's an exception in the 
//!				constructor of an object, instead of calling the regular delete overload.
//!				This function contains all the common code for 'array new' matching 'delete'
//=================================================================================================
void DeleteArrayCatchException(void* _pAlloc, zenMem::zAllocator* _pAllocator )
{
	if( _pAlloc )
	{
		zbMem::AllocHeader* pHeaderInfo = zbMem::GetHeader(_pAlloc, true);
		if( pHeaderInfo)
			pHeaderInfo->mpAllocator->Free(_pAlloc, pHeaderInfo);	
	}
}

void operator delete	(void* _pAlloc,	zenMem::zAllocator* _pAllocator )					{DeleteCatchException(_pAlloc, _pAllocator);}
void operator delete[]	(void* _pAlloc,	zenMem::zAllocator* _pAllocator )					{DeleteArrayCatchException(_pAlloc, _pAllocator);}
void operator delete	(void* _pAlloc,	zenMem::zAllocator* _pAllocator, zUInt _uAlign )	{DeleteCatchException(_pAlloc, _pAllocator);}
void operator delete[]	(void* _pAlloc,	zenMem::zAllocator* _pAllocator, zUInt _uAlign)	{DeleteArrayCatchException(_pAlloc, _pAllocator);}
