#pragma once
#ifndef __zenBase_Memory_Allocator_h__
#define __zenBase_Memory_Allocator_h__

#define		zenDel(_Pointer_)					{ delete	_Pointer_; }
#define		zenDelArray(_Pointer_)				{ delete[]	_Pointer_; }
#define		zenDelNull(_Pointer_)				{ delete	_Pointer_; _Pointer_=NULL; }
#define		zenDelNullArray(_Pointer_)			{ delete[]	_Pointer_; _Pointer_=NULL; }
#define		zenDefaultAlign						sizeof(zenPointer)
#define		zenNew(_Allocator_)					new(_Allocator_)
#define		zenNewAlign(_Allocator_, _Align_)	new(_Allocator_, _Align_)
#define		zenNewDefault						new(static_cast<zenMem::Allocator*>(NULL))
#define		zenNewDefaultAlign(_Align_)			new(static_cast<zenMem::Allocator*>(NULL), _Align_)

namespace zen { namespace zenMem
{
	class Allocator
	{
	AWClassDeclareNoParent(Allocator);
	public:
								Allocator			(zenDebugString _zName);		
		virtual					~Allocator			();
		size_t					GetTotalAllocSize	()const {return muTotalAllocSize;}
		zenUInt					GetTotalAllocCount	()const {return muTotalAllocCount;}
		inline zenHash32		GetValidityStamp	()const {return mh32ValidStamp;}
	protected:
		size_t					GetAllocSize		(size_t _uWantedSize, size_t _uExtraSize, zenU32 _uAlign);
		void*					AddAlloc			(size_t _uWantedSize, size_t _uExtraSize, zenU32 _uAlign, void* _pAllocation, const bool _bIsArray);
		void					RemAlloc			(void* _pAlloc);
		size_t					muTotalAllocSize;
		zenUInt					muTotalAllocCount;
		zenHash32				mh32ValidStamp;				
		zenList2x				mlstAllocations;	//!< List of every allocations currently held by this allocator (if compiled with MemoryDebugFlag)
		zenDebugString			mzAllocatorName;	//!< Name of the allocator, for debug purpose	
	
	// Implement on child class
	public:
		virtual void*			Malloc(size_t _uSize, bool _bIsArray, zenU32 _uAlign)=0;
		virtual	void			Free(void* _pAlloc, void* _pInfoAlloc)=0;
		virtual zenDebugString	GetDescription()=0;
	};

	/*
	//=================================================================================================
	//! @class		ScopedAllocator
	//-------------------------------------------------------------------------------------------------
	//! @brief		Change default allocator
	//! @details	While this object exist, replace the default allocator with the 
	//! @details	one specified. At object destruction, restore the value it was 
	//! @details	initially set at		
	//=================================================================================================
	class ScopedAllocator
	{	
	AWClassDeclareNoParent(ScopedAllocator);
	public:
		Override(AllocatorBase* _pAllocator)
 		: mpAllocatorPrevious(AllocatorBase::GetDefault())
		{
			AllocatorBase::Default = _pAllocator;
		}
		~Override()
		{
			AllocatorBase::Default = mpAllocatorPrevious;
		}	
	protected:
		AllocatorBase* mpAllocatorPrevious;
	};*/

}} //namespace zen { namespace mem  

//=============================================================================
// Override of builtin memory allocator
//=============================================================================
void* operator new(size_t _uSize,		zenMem::Allocator* _pAllocator );
void* operator new[](size_t _uSize,		zenMem::Allocator* _pAllocator );
void operator delete(void* _pAlloc,		zenMem::Allocator* _pAllocator );
void operator delete[](void* _pAlloc,	zenMem::Allocator* _pAllocator );
void* operator new(size_t _uSize,		zenMem::Allocator* _pAllocator, zenUInt _uAlign );
void* operator new[](size_t _uSize,		zenMem::Allocator* _pAllocator, zenUInt _uAlign );
void operator delete(void* _pAlloc,		zenMem::Allocator* _pAllocator, zenUInt _uAlign );
void operator delete[](void* _pAlloc,	zenMem::Allocator* _pAllocator, zenUInt _uAlign );
void operator delete(void* _pAlloc);
void operator delete[](void* _pAlloc);

void* zenMalloc(size_t _uSize_, zenU32 uAlign=zenDefaultAlign);
void* zenMalloc(zenMem::Allocator* _pAllocator, size_t _uSize_, zenU32 uAlign=zenDefaultAlign);

//! @todo remove new library and implement my own
//#define		malloc			AWStaticAssert(0, "Use zenMalloc")
//#define		new				AWAssertMsg(0, "Use zenNew")
//#define		delete			AWAssertMsg(0, "User zenDel")

#endif
