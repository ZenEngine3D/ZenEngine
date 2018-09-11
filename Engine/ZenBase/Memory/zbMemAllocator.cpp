#include "zbBase.h"

namespace zbMem
{

bool Allocator_Base::ShouldAccessCheck(size_t _Size, size_t _SizeMax, const char* _Filename, int _LineNumber, bool _IsArrayNew, bool _IsPoolItem, bool _IsCheckAccess) const
{
#if ZEN_MEMORY_SUPPORT_CHECKACCESS	
	return _IsCheckAccess; // Can implement additional logic based on allocation size, etc...
#else
	(void)_IsCheckAccess;
	return false;
#endif
}

void* Allocator::Malloc(size_t _Size, size_t _SizeMax, const char* _Filename, int _LineNumber, bool _IsArrayNew, bool _IsPoolItem, bool _IsCheckAccess)
{	
	_IsCheckAccess		= ShouldAccessCheck(_Size, _SizeMax, _Filename, _LineNumber, _IsArrayNew, _IsPoolItem, _IsCheckAccess);
	SAllocInfo Alloc	= Super::MallocInternal(mDebugTracking.GetSizeNeeded(_Size), mDebugTracking.GetSizeNeeded(_SizeMax), _IsPoolItem, _IsCheckAccess);
	return mDebugTracking.Malloc(Alloc, _Size, _SizeMax, _Filename, _LineNumber, _IsArrayNew, _IsCheckAccess);
}

void Allocator::Free(void* _pMemory, bool _IsArrayDel)
{
	if( _pMemory )
	{
		_pMemory = mDebugTracking.Free( _pMemory, _IsArrayDel );
		Super::FreeInternal(_pMemory);
	}
}

void* Allocator::Resize(void* _pMemory, size_t _NewSize)
{
	if( _pMemory )
	{
		void* pMemory		= mDebugTracking.PreResize(_pMemory, _NewSize);
		SAllocInfo Alloc	= Super::ResizeInternal(pMemory, mDebugTracking.GetSizeNeeded(_NewSize));		
		return mDebugTracking.PostResize(Alloc, _NewSize);
	}
	return nullptr;
}

size_t Allocator::GetRequestedSize(void* _pMemory)const
{
	void* pMemory			= mDebugTracking.GetMemory(_pMemory);
	size_t SizeRequested	= Super::GetRequestedSizeInternal(pMemory);
	return SizeRequested - mDebugTracking.GetSizeOverhead();
}

}  
