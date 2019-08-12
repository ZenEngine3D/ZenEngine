#pragma once

namespace zbMem
{

template<zInt TGroupCountMax, size_t TSizeMax>
PolicyPool<TGroupCountMax, TSizeMax>::PolicyPool()
{
	for(auto& GroupIndex : maPoolGroupIndexPerSize )
		GroupIndex = kPoolGroup_Invalid;
}

template<zInt TGroupCountMax, size_t TSizeMax>
const PoolGroup::Config& PolicyPool<TGroupCountMax, TSizeMax>::GetPoolGroup(size_t _Size)const
{
	zenAssert(_Size < TSizeMax);
	size_t SizeIndex = zenMath::RoundUp<size_t>(_Size, kAlign) / kAlign;
	return maPoolGroup[maPoolGroupIndexPerSize[SizeIndex]].GetConfig();
}

template<zInt TGroupCountMax, size_t TSizeMax>
bool PolicyPool<TGroupCountMax, TSizeMax>::IsBetterFit(zInt _GroupIndex, size_t _NewGroupSize)const
{	
	size_t NewSizeIndex		= _NewGroupSize / kAlign;
	size_t CurrentSizeIndex	= _GroupIndex != kPoolGroup_Invalid ? maPoolGroup[_GroupIndex].GetConfig().mItemSize / kAlign : 0;
	return	CurrentSizeIndex == 0 || float(CurrentSizeIndex) / float(NewSizeIndex) > 1.1f;
}

template<zInt TGroupCountMax, size_t TSizeMax>
bool PolicyPool<TGroupCountMax, TSizeMax>::ShouldAddGroup(size_t _Size)const
{
	return IsBetterFit(GetPoolGroup(_Size).mGroupIndex, _Size);
}

template<zInt TGroupCountMax, size_t TSizeMax>
void PolicyPool<TGroupCountMax, TSizeMax>::AddPoolGroup(const PoolGroup::Config& _Config)
{
	zenAssert(_Config.mItemSize % kAlign == 0);
	size_t NewSizeIndex		= _Config.mItemSize / kAlign;
	auto CurrentGroupIdx	= maPoolGroupIndexPerSize[NewSizeIndex];	
	if( IsBetterFit(CurrentGroupIdx,  _Config.mItemSize) )	
	{
		maPoolGroup[mPoolGroupCount].Initialize(_Config, mPoolGroupCount);		
		zInt CurrentSizeIndex = static_cast<zInt>(NewSizeIndex);
		while( CurrentSizeIndex >= 0 && IsBetterFit(maPoolGroupIndexPerSize[CurrentSizeIndex],  _Config.mItemSize) )
			maPoolGroupIndexPerSize[CurrentSizeIndex--] = mPoolGroupCount;

		mPoolGroupCount			= mPoolGroupCount + 1;
	}
}

template<zInt TGroupCountMax, size_t TSizeMax>
SAllocInfo PolicyPool<TGroupCountMax, TSizeMax>::Malloc(size_t _Size, size_t inItemCount, zenMem::AllocFlags inAllocFlags)
{	
	zenAssert(inItemCount == 1);
	const auto GroupIndex = GetPoolGroup(_Size).mGroupIndex;
	return maPoolGroup[GroupIndex].Malloc(_Size);
}

template<zInt TGroupCountMax, size_t TSizeMax>
void PolicyPool<TGroupCountMax, TSizeMax>::Free(void* _pMemory)
{
	zenAssert(_pMemory != nullptr);
	VAddressPool* pPoolInfo = reinterpret_cast<VAddressPool*>(&_pMemory);
	zenAssert((zInt)pPoolInfo->GroupIndex < mPoolGroupCount && pPoolInfo->PolicyType == kPolicy_Pool);
	maPoolGroup[pPoolInfo->GroupIndex].Free(_pMemory);
}

template<zInt TGroupCountMax, size_t TSizeMax>
SAllocInfo PolicyPool<TGroupCountMax, TSizeMax>::Resize(void* inpMemory, size_t inNewSize, size_t inItemCount)
{
	zenAssert(0); //Pool Policy doesn't support resize
	return SAllocInfo();
}

template<zInt TGroupCountMax, size_t TSizeMax>
size_t PolicyPool<TGroupCountMax, TSizeMax>::GetRequestedCount(void* _pMemory)
{
	return 1;
}

}  
