#pragma once

namespace zbMem
{

struct SAllocInfo
{
	SAllocInfo() = default;
	SAllocInfo(void* _pMemory, size_t _Size): pMemory(reinterpret_cast<zU8*>(_pMemory)), Size(_Size){}
	zU8*	pMemory	= nullptr;
	size_t	Size	= 0;
};

class IMemoryPolicy
{
public:		
	virtual SAllocInfo	Malloc(size_t _Size)=0;
	virtual SAllocInfo	Resize(void* _pMemory, size_t _NewSize)=0;
	virtual void		Free(void* _pMemory)=0;	
	virtual size_t		GetRequestedSize(void* _pMemory)=0;
};

}  
