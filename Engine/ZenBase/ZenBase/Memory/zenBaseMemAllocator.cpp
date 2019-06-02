#include "zbBase.h"

namespace zen { namespace zenMem
{

zbMem::Allocator& GetMemoryAllocator()
{
	static zbMem::Allocator MemoryAllocator;
	return MemoryAllocator;
}

// Note:	Classes with virtual table add some data at the beginning of memory(with most compilers) and object receive an offsetted address from new.
//			We ignore any part of the address that's not aligned to find the original memory address. 
//			Delete operator doesn't needs this since compiler remove the offset in generated code.
zenInline void* GetOriginalAddress(void* _pMemory)
{	
	_pMemory = reinterpret_cast<void*>(reinterpret_cast<zUInt>(_pMemory) & ~zUInt(zbMem::kAlign-1));
	return _pMemory;
}

size_t GetRequestedSize(void* _pMemory)
{
	return _pMemory ? GetMemoryAllocator().GetRequestedSize(GetOriginalAddress(_pMemory)) : 0;
}

void* ResizeMemory(void* _pMemory, size_t _NewSize)
{
#if ZEN_MEMORY_DEACTIVATE == 0
	zenAssert(_pMemory);
	return GetMemoryAllocator().Resize(GetOriginalAddress(_pMemory), _NewSize);
#else
	return realloc(_pMemory, _NewSize);
#endif
}

}}

void* operator new(size_t _Size, const char* _Filename, int _LineNumber, size_t _MaxSize, bool _IsPoolItem, bool _IsCheckAccess)
{
#if ZEN_MEMORY_DEACTIVATE == 0
	zenAssertMsg(_MaxSize == 0, "Resize not supported on single allocation, needs to be on array of objects");
	void* pMemory = zenMem::GetMemoryAllocator().Malloc(_Size, _Size, _Filename, _LineNumber, false, _IsPoolItem, _IsCheckAccess);
	return pMemory;
#else
	return malloc(_Size);
#endif
}

void* operator new[](size_t _Size, const char* _Filename, int _LineNumber, size_t _MaxSize, bool _IsPoolItem, bool _IsCheckAccess)
{
#if ZEN_MEMORY_DEACTIVATE == 0
	zenAssertMsg(_MaxSize == 0 || !_IsPoolItem, "Cannot have pool allocation with resize support");
	_MaxSize = zenMath::Max(_Size, _MaxSize);
	void* pMemory = zenMem::GetMemoryAllocator().Malloc(_Size, _MaxSize, _Filename, _LineNumber, true, _IsPoolItem, _IsCheckAccess);
	return pMemory;
#else
	return malloc(_Size);
#endif
}

void operator delete(void* _pMemory)
{
#if ZEN_MEMORY_DEACTIVATE == 0
	if( _pMemory )
		zenMem::GetMemoryAllocator().Free(_pMemory, false);
#else
	free(_pMemory);
#endif
}

void operator delete[](void* _pMemory)
{ 
#if ZEN_MEMORY_DEACTIVATE == 0
	if( _pMemory )
		zenMem::GetMemoryAllocator().Free(_pMemory, true);
#else
	free(_pMemory);
#endif
}

void* operator new(size_t _Size)
{
#if ZEN_MEMORY_DEACTIVATE == 0
	void* pMemory = zenMem::GetMemoryAllocator().Malloc(_Size, _Size, "", 0, false, false, false);
	return pMemory;
#else
	return malloc(_Size);
#endif
}

void* operator new[](size_t _Size)
{
#if ZEN_MEMORY_DEACTIVATE == 0
	void* pMemory = zenMem::GetMemoryAllocator().Malloc(_Size, _Size, "", 0, true, false, false);
	return pMemory;
#else
	return malloc(_Size);
#endif
}