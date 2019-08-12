#pragma once

namespace zbMem
{

class PolicyNativeMalloc : public IMemoryPolicy
{
zenClassDeclare(PolicyNativeMalloc, IMemoryPolicy);
public:
	virtual SAllocInfo						Malloc(size_t inSize, size_t inItemCount, zenMem::AllocFlags inAllocFlags) override;
	virtual SAllocInfo						Resize(void* inpMemory, size_t inNewSize, size_t inItemCount) override;
	virtual void							Free(void* inpMemory) override;	
	virtual size_t							GetRequestedCount(void* inpMemory)override;
};

}  
