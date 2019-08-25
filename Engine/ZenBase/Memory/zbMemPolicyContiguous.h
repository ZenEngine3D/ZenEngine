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
		zU32				mSizeWanted	= 0;
		zU32				mSizeUsed	= 0;
		zU32				mItemCount	= 0;
		zenMem::AllocFlags	mFlags;
		zU8					Padding[3];
	};
public:
										PolicyContiguous();										
	virtual SAllocInfo					Malloc(size_t inSize, size_t inItemCount, zenMem::AllocFlags inAllocFlags) override;
	virtual SAllocInfo					Resize(void* inpMemory, size_t inNewSize, size_t inItemCount) override;
	virtual void						Free(void* inpMemory) override;
	virtual size_t						GetRequestedCount(void* inpMemory)const override;
	virtual size_t						GetSupportedSizeMax()const override;

	const AllocInfo&					GetAllocInfo(void* inpMemory)const;
protected:
	zArrayFixed<AllocInfo, TAllocMax>	maAllocatedInfo;
	zArrayFixed<zU32, TAllocMax>		maAvailableIndices;
	std::atomic<zU32>					mAvailableHead;
	std::atomic<zU32>					mAvailableTail;
	size_t								mVirtualMemorySize;
	size_t								mPhysicalPageSize;		
};

}  
