#pragma once

#include "zbMemPoolGroup.h"

namespace zbMem
{

template<zInt TGroupCountMax, size_t TSizeMax>
class PolicyPool : public IMemoryPolicy
{
zenClassDeclare(PolicyPool, IMemoryPolicy);
public:
											PolicyPool();
	void									AddPoolGroup(const PoolGroup::Config& _Config);
	zenInline const PoolGroup::Config&		GetPoolGroup(size_t _Size)const;
	zenInline bool							ShouldAddGroup(size_t _AlignedSize)const;

	virtual SAllocInfo						Malloc(size_t inSize, size_t inItemCount, zenMem::AllocFlags inAllocFlags) override;
	virtual SAllocInfo						Resize(void* inpMemory, size_t inNewSize, size_t inItemCount) override;
	virtual void							Free(void* inpMemory) override;	
	virtual size_t							GetRequestedCount(void* inpMemory)const override;
	virtual size_t							GetSupportedSizeMax()const override;
protected:	
	zenInline bool							IsBetterFit(zInt _GroupIndex, size_t _NewGroupAlignedSize)const;

	enum Constants{kPoolGroup_Invalid=-1};
	std::atomic<zInt>						mPoolGroupCount=0;
	std::array<PoolGroup, TGroupCountMax>	maPoolGroup;				//!< List of current pool group
	std::array<zInt, (TSizeMax/kAlign)+1>	maPoolGroupIndexPerSize;	//!< Quick lookup of PoolGroup Index for wanted allocation size
};

}  
