#pragma once

namespace zbMem
{
	class Allocator_Base
	{
	zenClassDeclareNoParent(Allocator_Base);
	public:
						Allocator_Base()=default;	
	protected:		
		bool			ShouldAccessCheck(size_t inSize, size_t inItemCount, const zenMem::AllocFlags& inAllocFlags)const;
		DebugTracking	mDebugTracking;
	};
}  

#include zenHeaderPlatform(zbMemAllocator)

namespace zbMem
{
	class Allocator : public Allocator_HAL
	{
	zenClassDeclare(Allocator, Allocator_HAL);	
	public:	
						Allocator()=default;
		void*			Malloc(size_t inSize, size_t inItemCount, zenMem::AllocFlags inAllocFlags);
		void* 			Resize(void* _pMemory, size_t _NewSize, size_t _ItemCount);		
		void			Free(void* _pMemory);
		size_t			GetItemCount(void* _pMemory)const;	
	};
}
