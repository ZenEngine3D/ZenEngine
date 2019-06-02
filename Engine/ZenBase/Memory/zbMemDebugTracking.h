#pragma once

namespace zbSys { class Callstack; } //Forward declare

namespace zbMem
{
	class DebugTracking
	{
	zenClassDeclareNoParent(DebugTracking);
	public:
								DebugTracking();
		zenInline void*			Malloc(const SAllocInfo& _Allocation, size_t _SizeWanted, size_t _SizeMax, const char* _Filename, int _LineNumber, bool _IsArrayNew, bool _IsTrackingBadAccess);
		zenInline void*			PreResize(void* _pMemory, size_t _NewSize);
		zenInline void*			PostResize(const SAllocInfo& Alloc, size_t _NewSize);
		zenInline void*			Free(void* _pMemory, bool _IsArrayDel);
		zenInline static void*	GetMemory(void* _pMemory);
		zenInline static size_t	GetSizeNeeded(size_t _SizeWanted);
		static constexpr size_t	GetSizeOverhead();

#if ZEN_MEMORY_TRACKING	
	protected:
		zenInline static void	CheckValidity(const void* _pMemory);

		struct Header
		{	
			zListLinkMember(1);							//16
			const zbSys::Callstack*	mpCallstack;		//24
			const char*				mFileName;			//32
			zU32					mFileLine;			//36					
			zU32					mSizeWanted;		//40
			zU32					mSizeUsed;			//44
			zU8						mIsArrayNew;		//45
			zU8						mIsCheckAccess;		//46
			zU8						_Padding[2];		//47,48
			using List = zList<Header>;
		};

		struct Footer
		{
			Footer();
			std::array<zU8,zbMem::kAlign> maFooter;
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
