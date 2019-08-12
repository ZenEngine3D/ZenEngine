#pragma once


namespace zen{ namespace zenMem
{
	enum eAllocFlags
	{
		keFlag_NativeMalloc,	// Force native OS allocation using malloc (useful for Visual Studio Allocation tracking, but slower)
		keFlag_Resize,			// Need fast resize support
		keFlag_ResizeLarge,		// Need fast resize support, with high amount of memory potentially needed
		keFlag_Pool,			// Want fast small single element allocation
		keFlag_Protected,		// Activate tracking of buffer overrun and memory use after released (requires more memory, requiring a page size multiple allocation)
		keFlag__Count
	};

	using AllocFlags = zFlag<eAllocFlags, eAllocFlags::keFlag__Count, zU8>;
}}

#define zenVariableSize						1 //Move this elsewhere

namespace zbMem
{
	template<class TType, typename... TConstrParams>
	TType* Construct(void* inpMemory, size_t inCount, const TConstrParams&... inConstrParams)
	{
		TType* pNewObj = reinterpret_cast<TType*>(inpMemory);
	
		// If there's no constructors parameters and object has empty constructor, skip loop to init individual items
		constexpr bool bCanSkipInit = sizeof...(inConstrParams) == 0 && std::is_trivially_constructible<TType>::value;
		if( bCanSkipInit == false )
		{
			for(size_t i=0; i<inCount; ++i)
			{
				new(&pNewObj[i]) TType(inConstrParams...);
			}
		}	
		return pNewObj;	
	}

	template<class TType>
	void HandleMoved(TType* inpPrevious, TType* inpUpdated, size_t inPreviousCount, size_t inUpdatedCount)
	{
		if( inpPrevious != inpUpdated )
		{
			const zInt copyCount = zenMath::Min<zInt>(inPreviousCount, inUpdatedCount);
			if( std::is_trivially_copyable<TType>::value == false )
			{
				for(zInt i(0); i<copyCount; ++i)
					new(&inpUpdated[i]) TType(inpPrevious[i]);
			}
			else
			{
				memcpy(inpUpdated, inpPrevious, sizeof(TType)*copyCount );
			}

			if( std::is_trivially_destructible<TType>::value == false )
			{
				for(zInt i(0); i<copyCount; ++i)
					inpPrevious[i].~TType();
			}
			zbMem::Deallocate(inpPrevious);
		}
	}

	void*	Allocate		(size_t inSize, size_t inItemCount, zenMem::AllocFlags inAllocFlags);
	void*	ResizeMemory	(void* _pMemory, size_t _NewSize, size_t inItemCount);
	void	Deallocate		(void* _pMemory);	
	size_t	GetItemCount	(void* _pMemory);	
}

namespace zen{ 
namespace zenMem
{	
	template<class TType, typename... TConstrParams>
	TType* New(const TConstrParams&... inConstrParams)
	{
		void* pMem = zbMem::Allocate(sizeof(TType), 1, AllocFlags());
		return zbMem::Construct<TType, TConstrParams...>(pMem, 1, inConstrParams...);
	}

	template<class TType, typename... TConstrParams>
	TType* NewArray(size_t inCount, const TConstrParams&... inConstrParams)
	{
		void* pMem = zbMem::Allocate(sizeof(TType)*inCount, inCount, AllocFlags());
		return zbMem::Construct<TType, TConstrParams...>(pMem, inCount, inConstrParams...);
	}

	template<class TType, typename... TConstrParams>
	TType* NewProtected(size_t inCount, const TConstrParams&... inConstrParams)
	{
		void* pMem = zbMem::Allocate(sizeof(TType)*inCount, inCount, AllocFlags(keFlag_Protected));
		return zbMem::Construct<TType, const TConstrParams&...>(pMem, inCount, inConstrParams...);
	}

	template<class TType, typename... TConstrParams>
	TType* NewResizeable(size_t inCount, const TConstrParams&... inConstrParams)
	{
		void* pMem = zbMem::Allocate(sizeof(TType)*inCount, inCount, AllocFlags(keFlag_Resize));
		return zbMem::Construct<TType, const TConstrParams&...>(pMem, inCount, inConstrParams...);
	}

	template<class TType, typename... TConstrParams>
	TType* NewResizeableLarge(size_t inCount, const TConstrParams&... inConstrParams)
	{
		void* pMem = zbMem::Allocate(sizeof(TType)*inCount, inCount, AllocFlags(keFlag_ResizeLarge));
		return zbMem::Construct<TType, const TConstrParams&...>(pMem, inCount, inConstrParams...);
	}

	template<class TType, typename... TConstrParams>
	TType* NewPool(const TConstrParams&... inConstrParams)
	{
		void* pMem = zbMem::Allocate(sizeof(TType), 1, AllocFlags(keFlag_Pool));
		return zbMem::Construct<TType, const TConstrParams&...>(pMem, 1, inConstrParams...);
	}

	template<class TType, typename... TConstrParams>
	TType* Resize(TType* inpObject, size_t inItemCount, const TConstrParams&... inConstrParams)
	{	
		zInt OldItemCount	= (zInt)zbMem::GetItemCount(inpObject);
		zInt CountChange	= (zInt)inItemCount - (zInt)OldItemCount;
		if( CountChange == 0 )
			return inpObject;

		// Call destructor on removed items when memory has shrunk
		if( std::is_trivially_destructible<TType>::value == false )
		{
			while( CountChange < 0 )
				inpObject[OldItemCount + CountChange++].~TType();
		}

		TType* pReallocObject = reinterpret_cast<TType*>(zbMem::ResizeMemory(inpObject, sizeof(TType)*inItemCount, inItemCount));
		
		// Special handling if content has moved (should only happen when using OS malloc)
		zbMem::HandleMoved(inpObject, pReallocObject, OldItemCount, inItemCount); 
		
		// Call constructor on new items when memory had grown
		if( CountChange > 0 )
			zbMem::Construct<TType, TConstrParams...>(&pReallocObject[OldItemCount], CountChange, inConstrParams...);

		return pReallocObject;
	}

	template<class TType>
	void Del(TType* inpObject)
	{
		if( inpObject != nullptr )
		{
			if( std::is_trivially_destructible<TType>::value == false )
			{
				size_t CurrentCount	= zbMem::GetItemCount(inpObject);
				while( CurrentCount > 0 )
					inpObject[--CurrentCount].~TType();
			}
			zbMem::Deallocate(inpObject);
		}
	}

	template<class TType>
	void DelSafe(TType*& inpObject)
	{
		Del(inpObject);
		inpObject = nullptr;
	}
} }