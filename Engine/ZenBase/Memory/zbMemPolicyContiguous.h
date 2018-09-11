#pragma once

namespace zbMem
{

template<class TVirtualAdrInfo, zU32 TAllocMax, zUInt TAllocatorType>
class PolicyContiguous : public IMemoryPolicy
{
zenClassDeclare(PolicyContiguous, IMemoryPolicy);
public:
	struct AllocInfo
	{
		size_t	mSizeWanted	= 0;
		size_t	mSizeUsed	= 0;
	};
public:
										PolicyContiguous();										
	virtual SAllocInfo					Malloc(size_t _Size) override;
	virtual SAllocInfo					Resize(void* _pMemory, size_t _NewSize) override;
	virtual void						Free(void* _pMemory) override;
	virtual size_t						GetRequestedSize(void* _pMemory)override;
	
	const AllocInfo&					GetAllocInfo(void* _pMemory)const;
	zenInline size_t					GetSupportedSizeMax()const;
	
protected:
	std::array<AllocInfo, TAllocMax>	maAllocatedInfo;
	std::array<zU32, TAllocMax>			maAvailableIndices;
	std::atomic<zU32>					mAvailableHead;
	std::atomic<zU32>					mAvailableTail;
	size_t								mVirtualMemorySize;
	size_t								mPhysicalPageSize;		
};

}  
