#include "libZenBase.h"

void* operator new(size_t _uSize, CMem::Allocator* _pAllocator)
{
	_pAllocator = _pAllocator ? _pAllocator : &CMem::Allocator::GetDefault();
	return _pAllocator->Malloc(_uSize, false, awDefaultAlign);
}

void* operator new[](size_t _uSize, CMem::Allocator* _pAllocator )
{
	_pAllocator = _pAllocator ? _pAllocator : &CMem::Allocator::GetDefault();
	return _pAllocator->Malloc(_uSize, true, awDefaultAlign);
}

void* operator new(size_t _uSize, CMem::Allocator* _pAllocator, awUInt _uAlign )
{
	_pAllocator = _pAllocator ? _pAllocator : &CMem::Allocator::GetDefault();
	return _pAllocator->Malloc(_uSize, false, _uAlign);
}

void* operator new[](size_t _uSize, CMem::Allocator* _pAllocator, awUInt _uAlign )
{
	_pAllocator = _pAllocator ? _pAllocator : &CMem::Allocator::GetDefault();
	return _pAllocator->Malloc(_uSize, true, _uAlign);
}

void operator delete(void* _pAlloc)
{
	if( _pAlloc )
	{
		CMem::Allocator::Header* pHeaderInfo = CMem::Allocator::GetHeader(_pAlloc, false);
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
		CMem::Allocator::Header* pHeaderInfo = CMem::Allocator::GetHeader(_pAlloc, true);
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
void DeleteCatchException(void* _pAlloc, CMem::Allocator* _pAllocator )
{
	if( _pAlloc )
	{
		CMem::Allocator::Header* pHeaderInfo = CMem::Allocator::GetHeader(_pAlloc, false);
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
void DeleteArrayCatchException(void* _pAlloc, CMem::Allocator* _pAllocator )
{
	if( _pAlloc )
	{
		CMem::Allocator::Header* pHeaderInfo = CMem::Allocator::GetHeader(_pAlloc, true);
		if( pHeaderInfo)
			pHeaderInfo->mpAllocator->Free(_pAlloc, pHeaderInfo);	
	}
}

void operator delete(void* _pAlloc,	CMem::Allocator* _pAllocator )					{DeleteCatchException(_pAlloc, _pAllocator);}
void operator delete[](void* _pAlloc, CMem::Allocator* _pAllocator )				{DeleteArrayCatchException(_pAlloc, _pAllocator);}
void operator delete(void* _pAlloc, CMem::Allocator* _pAllocator, awUInt _uAlign )	{DeleteCatchException(_pAlloc, _pAllocator);}
void operator delete[](void* _pAlloc, CMem::Allocator* _pAllocator, awUInt _uAlign)	{DeleteArrayCatchException(_pAlloc, _pAllocator);}
