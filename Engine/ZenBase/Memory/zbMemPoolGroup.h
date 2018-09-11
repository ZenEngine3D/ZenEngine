#pragma once

namespace zbMem
{
struct VAddressPool_PC
{
	zU64 SubAddress		: 32;
	zU64 GroupIndex		: 8;
	zU64 _Unused		: 5;
	zU64 PolicyType		: 2;
	zU64 Invalid		: 17;
	operator void*() { return *reinterpret_cast<void**>(this); }
};
using VAddressPool = VAddressPool_PC;

class PoolGroup 
{
zenClassDeclare(PoolGroup, IMemoryPolicy);
public:
	struct Config
	{
		size_t	mPageSize				= 0;
		size_t	mItemSize				= 0;
		zU8		mGroupIndex				= 0;
		zU8		mPolicyType				= 0; 
	};
public:
	void						Initialize(const Config& _Config, zInt _GroupIndex);
	zenInline bool				IsInitialized()const;
	zenInline const Config&		GetConfig()const{return mConfig;}

	SAllocInfo					Malloc(size_t _Size);
	void						Free(void* _pMemory);

protected:
	enum Constants{kMaxThread=128};
	void						GrowPool();
	Config						mConfig;
	VAddressPool				mVirtualAddressStart;
	size_t						mVirtualMemorySize		= 0;
	size_t						mPhysicalAllocatedSize	= 0;
	zListElement::List			mlstFree[kThreadCount];
};

}  
