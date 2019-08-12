#pragma once

namespace zbSys { class Callstack; } //Forward declare

namespace zbMem
{
	class DebugTracking
	{
	zenClassDeclareNoParent(DebugTracking);
	public:
								DebugTracking();
		zenInline void*			Malloc(const SAllocInfo& _Allocation, size_t _SizeWanted, zenMem::AllocFlags inAllocFlags);
		zenInline void*			PreResize(void* _pMemory, size_t _NewSize);
		zenInline void*			PostResize(void* _pPreResizeAlloc, const SAllocInfo& _Alloc, size_t _SizeWanted);
		zenInline void*			Free(void* _pMemory);
		zenInline static void*	GetMemory(void* _pMemory);
		zenInline static size_t	GetSizeNeeded(size_t _SizeWanted);
		static constexpr size_t	GetSizeOverhead();

#if ZEN_MEMORY_TRACKING	
	protected:
		zenInline static void	CheckValidity(const void* _pMemory);

		struct Header
		{	
			zListLinkMember(1);						//16
			const zbSys::Callstack*	mpCallstack;	//24
			zU32					mSizeWanted;	//28
			zU32					mSizeUsed;		//32
			zU32					mItemCount;		//36
			zenMem::AllocFlags		mAllocFlags;	//37
			zU8						Padding[11];	//38...48
			using List = zList<Header>;
		};

		struct Footer
		{
			Footer();
			std::array<zU8, zbMem::kAlign> maFooter;
		};

		struct Stats
		{
			zU64	mAllocationCount	= 0;			
			zU64	mSizeWanted			= 0;
			zU64	mSizeUsed			= 0;
		};

		Stats			mStats;		
		Header::List	mAllocatedItems;		
#endif
	};	
}  

#include "zbMemDebugTracking.inl"
