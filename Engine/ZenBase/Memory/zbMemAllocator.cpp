#include "zbBase.h"

namespace zbMem
{

void* Allocator::Malloc(size_t inSize, size_t inItemCount, zenMem::AllocFlags inAllocFlags)
{		
	SAllocInfo Alloc = Super::MallocInternal(mDebugTracking.GetSizeNeeded(inSize), inItemCount, inAllocFlags);
	return mDebugTracking.Malloc(Alloc, inSize, inAllocFlags);
}

void Allocator::Free(void* _pMemory)
{
	if( _pMemory )
	{
		_pMemory = mDebugTracking.Free(_pMemory);
		Super::FreeInternal(_pMemory);
	}
}

void* Allocator::Resize(void* _pMemory, size_t _NewSize, size_t _ItemCount)
{
	if( _pMemory )
	{
		void* pMemory		= mDebugTracking.PreResize(_pMemory, _NewSize);
		SAllocInfo Alloc	= Super::ResizeInternal(pMemory, mDebugTracking.GetSizeNeeded(_NewSize), _ItemCount);		
		return mDebugTracking.PostResize(_pMemory, Alloc, _NewSize);
	}
	return nullptr;
}

size_t Allocator::GetItemCount(void* _pMemory)const
{
	void* pMemory = mDebugTracking.GetMemory(_pMemory);
	return Super::GetItemCountInternal(pMemory);
}

}  
