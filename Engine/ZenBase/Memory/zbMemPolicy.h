#pragma once

namespace zbMem
{

struct SAllocInfo
{
	SAllocInfo() = default;
	SAllocInfo(void* inpMemory, size_t inSize): pMemory(reinterpret_cast<zU8*>(inpMemory)), Size(inSize){}
	zU8*	pMemory	= nullptr;
	size_t	Size	= 0;
};

class IMemoryPolicy
{
public:		
	virtual SAllocInfo	Malloc(size_t inSize, size_t inItemCount, zenMem::AllocFlags inAllocFlags)=0;
	virtual SAllocInfo	Resize(void* inpMemory, size_t inNewSize, size_t inItemCount)=0;
	virtual void		Free(void* inpMemory)=0;	
	virtual size_t		GetRequestedCount(void* inpMemory)const=0;
	virtual size_t		GetSupportedSizeMax()const=0;
};

}  
