#pragma once
#ifndef __LibCore_Memory_Allocator_h__
#define __LibCore_Memory_Allocator_h__

namespace CMem
{
	//=================================================================================================
	//! @class		Allocator
	//-------------------------------------------------------------------------------------------------
	//! @brief		Base class each memory allocator type must implement
	//! @details	Take care of memory header every memory allocation has		
	//=================================================================================================
	class Allocator
	{
	AWClassDeclareNoParent(Allocator);
	public:
		//=================================================================================================
		//! @class		Header
		//-------------------------------------------------------------------------------------------------
		//! @brief		Informations about a memory allocation
		//! @details	Every allocation has this layout [Optional][Header][Memory][Footer]
		//!				where... 
		//!					-# Optional : Per allocator type infos (can be empty)
		//!					-# Header : Infos on the allocation
		//!					-# Memory : User requested memory
		//!					-# Footer : Padding added at the end to detect buffer overrun
		//! @todo		Reduce memory footprint by packing together infos and removing debug only infos 
		//! @todo		Might not need to keep track of every allocation (keep it only as memtracking define)
		//=================================================================================================
		class Header
	#if AW_MEMORYDEBUG
		: public zenList2xNode
		{
		AWClassDeclare(Header, zenList2xNode);
	#else
		{
		AWClassDeclareNoParent(Header);
	#endif
		public:			
			Allocator*			mpAllocator;
			size_t				muWantedSize;
			zenHash32			mhStamp;
			zenU32				muOffset : 31;
			zenU32				mbIsArray: 1;
			inline bool			IsValid(){return mhStamp==zenHash32("ValidAlloc");}
			bool				IsArray();
			void				Set(Allocator* _pAllocator, zenU32 _uAllocOffset, size_t _uAllocSize, bool _bIsArray);
		};			
								Allocator			(zenDebugString _zName);		
		virtual					~Allocator			();
		virtual void			DebugPrint			();
		size_t					GetTotalAllocSize	()const {return muTotalAllocSize;}
		zenUInt					GetTotalAllocCount	()const {return muTotalAllocCount;}
		static Header*			GetHeader			(void* _pAlloc, bool _bIsArray);		
		static Allocator&		GetDefault			();		
		static Allocator*		Default;			//!< Engine default allocator to use @todo Allow alloc to use NULL instead of frocing them to use directly this
	protected:
		size_t					GetAllocSize		(size_t _uWantedSize, size_t _uExtraSize, zenU32 _uAlign);
		void*					AddAlloc			(size_t _uWantedSize, size_t _uExtraSize, zenU32 _uAlign, void* _pAllocation, const bool _bIsArray);
		void					RemAlloc			(Header* _pAlloc);
		size_t					muTotalAllocSize;
		zenUInt					muTotalAllocCount;
		zenHash32					mh32ValidStamp;				
	#if AW_MEMORYDEBUG
		zenList2x					mlstAllocations;	//!< List of every allocation currently held by this allocator
	#endif
		zenDebugString				mzAllocatorName;	//!< Name of the allocator, for debug purpose
	// Methods to implement
	public:
		virtual void*			Malloc(size_t _uSize, bool _bIsArray, zenU32 _uAlign)=0;
		virtual	void			Free(void* _pAlloc, Header* _pInfoAlloc)=0;
		virtual zenDebugString		GetDescription()=0;
	};

	//=================================================================================================
	//! @class		Override
	//-------------------------------------------------------------------------------------------------
	//! @brief		Change default allocator
	//! @details	While this object exist, replace the default allocator with the 
	//! @details	one specified. At object destruction, restore the value it was 
	//! @details	initially set at		
	//=================================================================================================
	class Override
	{	
	AWClassDeclareNoParent(Override);
	public:
		Override(Allocator* _pAllocator)
 		: mpAllocatorPrevious(Allocator::Default)
		{
			Allocator::Default = _pAllocator;
		}
		~Override()
		{
			Allocator::Default = mpAllocatorPrevious;
		}	
	protected:
		Allocator* mpAllocatorPrevious;
	};
}  

#endif
