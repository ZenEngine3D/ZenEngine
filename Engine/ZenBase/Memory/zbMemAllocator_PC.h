#pragma once

namespace zbMem
{
	struct VAddressCommonPC
	{
		zU64 Available		: 45;
		zU64 AllocatorType	: 2;
		zU64 Invalid		: 17;
		operator void*() { return *reinterpret_cast<void**>(this); }
	};

	struct VAddressRegularPC
	{
		zU64 SubAddress		: 25;
		zU64 AllocationIndex: 20;
		zU64 AllocatorType	: 2;
		zU64 Invalid		: 17;
		operator void*() { return *reinterpret_cast<void**>(this); }
	};

	struct VAddressLargePC
	{
		zU64 SubAddress		: 30;
		zU64 AllocationIndex: 15;
		zU64 AllocatorType	: 2;
		zU64 Invalid		: 17;
		operator void*() { return *reinterpret_cast<void**>(this); }
	};
	
	enum PolicyType
	{	
		kPolicy_NativeMalloc	= 0,
		kPolicy_Pool			= 1,
		kPolicy_Regular			= 2,
		kPolicy_Large			= 3,
		kPolicy__Count,		
		kPolicy__Invalid,
	};
	
	class Allocator_PC : public Allocator_Base
	{		
	zenClassDeclare(Allocator_PC, Allocator_Base);
	protected:
	enum Constants
	{		 
		kPoolGroupMax=1<<8,
		kPoolItemSizeMax=2048+DebugTracking::GetSizeOverhead()
	};
								Allocator_PC();
	protected:
		//------------------------------------------------------------------------------------------
		// Avoids virtual interface, but each platform must implement these methods
		SAllocInfo				MallocInternal	(size_t inSize, size_t inItemCount, zenMem::AllocFlags inAllocFlags);
		SAllocInfo				ResizeInternal	(void* _pMemory, size_t _NewSize, size_t inItemCount);
		void					FreeInternal	(void* _pMemory);		
		size_t					GetItemCountInternal(void* _pMemory)const;
		//------------------------------------------------------------------------------------------

		std::array<IMemoryPolicy*, kPolicy__Count>						maPolicyAll;
		PolicyNativeMalloc												mPolicyNative;
		PolicyPool<kPoolGroupMax,kPoolItemSizeMax>						mPolicyPool;
		PolicyContiguous<VAddressRegularPC, (1<<19), kPolicy_Regular>	mPolicyRegular;		
		PolicyContiguous<VAddressLargePC, (1<<13), kPolicy_Large>		mPolicyLarge;
	};
	using Allocator_HAL = Allocator_PC;
}  
