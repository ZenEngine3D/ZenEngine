#pragma once
#ifndef __zenApi_Base_Memory_Allocator_h__
#define __zenApi_Base_Memory_Allocator_h__

#define		zenDel(_Pointer_)					{ delete	_Pointer_; }
#define		zenDelArray(_Pointer_)				{ delete[]	_Pointer_; }
#define		zenDelnullptr(_Pointer_)			{ delete	_Pointer_; _Pointer_=nullptr; }
#define		zenDelnullptrArray(_Pointer_)		{ delete[]	_Pointer_; _Pointer_=nullptr; }
#define		zenDefaultAlign						sizeof(void*)
#define		zenNew(_Allocator_)					new(_Allocator_)
#define		zenNewAlign(_Allocator_, _Align_)	new(_Allocator_, _Align_)
#define		zenNewDefault						new(static_cast<zenMem::zAllocator*>(nullptr))
#define		zenNewDefaultAlign(_Align_)			new(static_cast<zenMem::zAllocator*>(nullptr), _Align_)

namespace zen { namespace zenMem
{

	class zAllocator
	{
	ZENClassDeclareNoParent(zAllocator);
	public:
		struct HeaderListItem
		{
		#if ZEN_MEMORYDEBUG_ON
			zListLink mlstLink;
			typedef zListLink<HeaderListItem, &HeaderListItem::mlstLink> TypeList;
		#endif
		};
									zAllocator			(zDebugString _zName);		
		virtual						~zAllocator			();
		size_t						GetTotalAllocSize	()const {return muTotalAllocSize;}
		zUInt						GetTotalAllocCount	()const {return muTotalAllocCount;}
		ZENInline zHash32			GetValidityStamp	()const {return mh32ValidStamp;}
		static zAllocator&			GetDefault			();
	
	protected:
		size_t					GetAllocSize		(size_t _uWantedSize, size_t _uExtraSize, zU32 _uAlign);
		void*					AddAlloc			(size_t _uWantedSize, size_t _uExtraSize, zU32 _uAlign, void* _pAllocation, const bool _bIsArray);
		void						RemAlloc			(void* _pAlloc);
		size_t						muTotalAllocSize;
		zUInt						muTotalAllocCount;
		zHash32						mh32ValidStamp;
	#if ZEN_MEMORYDEBUG_ON
		HeaderListItem::TypeList	mlstAllocations;	//!< List of every allocations currently held by this allocator (if compiled with MemoryDebugFlag)
	#endif
		zDebugString				mzAllocatorName;	//!< Name of the allocator, for debug purpose	
	
	// Implement on child class
	public:
		virtual void*			Malloc(size_t _uSize, bool _bIsArray, zU32 _uAlign)=0;
		virtual	void				Free(void* _pAlloc, void* _pInfoAlloc)=0;
		virtual zDebugString		GetDescription()=0;
	};

	
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
	ZENClassDeclareNoParent(ScopedAllocator);
	public:
								ScopedAllocator(zAllocator* _pAllocator);
								~ScopedAllocator();
		static zAllocator&		GetActive();
	
	protected:
		zAllocator*				mpAllocator;
		zListLink				mlstLink;
	public:
		typedef zList<ScopedAllocator, &ScopedAllocator::mlstLink> TypeList;
	};

}} //namespace zen { namespace mem  

//=============================================================================
// Override of builtin memory allocator
//=============================================================================
void* operator new(size_t _uSize,		zenMem::zAllocator* _pAllocator );
void* operator new[](size_t _uSize,		zenMem::zAllocator* _pAllocator );
void operator delete(void* _pAlloc,		zenMem::zAllocator* _pAllocator );
void operator delete[](void* _pAlloc,	zenMem::zAllocator* _pAllocator );
void* operator new(size_t _uSize,		zenMem::zAllocator* _pAllocator, zU32 _uAlign );
void* operator new[](size_t _uSize,		zenMem::zAllocator* _pAllocator, zU32 _uAlign );
void operator delete(void* _pAlloc,		zenMem::zAllocator* _pAllocator, zU32 _uAlign );
void operator delete[](void* _pAlloc,	zenMem::zAllocator* _pAllocator, zU32 _uAlign );
void operator delete(void* _pAlloc);
void operator delete[](void* _pAlloc);

void* zMalloc(size_t _uSize_, zU32 uAlign=zenDefaultAlign);
void* zMalloc(zenMem::zAllocator* _pAllocator, size_t _uSize_, zU32 uAlign=zenDefaultAlign);

//! @todo remove new library and implement my own
//#define		malloc			ZENStaticAssert(0, "Use zMalloc")
//#define		new				ZENAssertMsg(0, "Use zenNew")
//#define		delete			ZENAssertMsg(0, "User zenDel")

#endif
