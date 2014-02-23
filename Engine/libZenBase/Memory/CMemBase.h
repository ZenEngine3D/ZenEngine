#pragma once
#ifndef __LibCore_Memory_Base_h__
#define __LibCore_Memory_Base_h__

#include "CMemAllocator.h"

#define		AWDel(_Pointer_)			{ delete	_Pointer_; }
#define		AWDelArray(_Pointer_)		{ delete[]	_Pointer_; }
#define		AWDelNull(_Pointer_)		{ delete	_Pointer_; _Pointer_=NULL; }
#define		AWDelNullArray(_Pointer_)	{ delete[]	_Pointer_; _Pointer_=NULL; }
#define		awDefaultAlign				sizeof(zenPointer)

#define		AWNew(_Allocator_)			new(_Allocator_)
#define		AWNewDefault				new(static_cast<CMem::Allocator*>(NULL))
#define		AWNewAlign(_Align_)			new(_Allocator_, _Align_)
#define		AWNewDefaultAlign(_Align_)	new(CMem::Allocator::GetDefault(), _Align_)

void* operator new(size_t _uSize,		CMem::Allocator* _pAllocator );
void* operator new[](size_t _uSize,		CMem::Allocator* _pAllocator );
void operator delete(void* _pAlloc,		CMem::Allocator* _pAllocator );
void operator delete[](void* _pAlloc,	CMem::Allocator* _pAllocator );

void* operator new(size_t _uSize,		CMem::Allocator* _pAllocator, zenUInt _uAlign );
void* operator new[](size_t _uSize,		CMem::Allocator* _pAllocator, zenUInt _uAlign );
void operator delete(void* _pAlloc,		CMem::Allocator* _pAllocator, zenUInt _uAlign );
void operator delete[](void* _pAlloc,	CMem::Allocator* _pAllocator, zenUInt _uAlign );

void operator delete(void* _pAlloc);
void operator delete[](void* _pAlloc);

inline void* AWMalloc(size_t _uSize_, zenU32 uAlign=awDefaultAlign)
{
	return CMem::Allocator::GetDefault().Malloc(_uSize_, false, uAlign);
}

inline void* AWMalloc(CMem::Allocator* _pAllocator, size_t _uSize_, zenU32 uAlign=awDefaultAlign)
{
	_pAllocator = _pAllocator ? _pAllocator : &CMem::Allocator::GetDefault();
	return _pAllocator->Malloc(_uSize_, false, uAlign);
}

#define		AWFree(_PTR)				free(_PTR)


//! @todo remove new library and implement my own
//#define		malloc			AWStaticAssert(0, "Use AWMalloc")
//#define		new				AWAssertMsg(0, "Use AWNew")
//#define		delete			AWAssertMsg(0, "User AWDel")

#endif
