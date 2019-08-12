#include "zbBase.h"

namespace zbMem
{

struct MallocHeader
{
	enum { kStamp=0xBABA };	
	zU32				mSize;
	zU32				mSizeExtra;	
	zU32				mItemCount;
	zenMem::AllocFlags	mFlags;
	zU8					mPadding;
	zU16				mStamp;

	bool IsValid()const { return mStamp==(zU16)kStamp; }
};

SAllocInfo PolicyNativeMalloc::Malloc(size_t inSize, size_t inItemCount, zenMem::AllocFlags inAllocFlags)
{	
	zenStaticAssert(sizeof(MallocHeader) == zbMem::kAlign);
	SAllocInfo AllocInfo;
	size_t SizeTotal		= inAllocFlags.Any(zenMem::keFlag_Resize,zenMem::keFlag_Resize) ? inSize * 4 / 3 : inSize;
	SizeTotal				= zenMath::RoundUp<size_t>(SizeTotal+sizeof(MallocHeader), zbMem::kAlign);
	MallocHeader* pHeader	= reinterpret_cast<MallocHeader*>(malloc(SizeTotal));
	pHeader->mStamp			= (zU16)MallocHeader::kStamp;
	pHeader->mFlags			= inAllocFlags;
	pHeader->mSize			= static_cast<zU32>(inSize);
	pHeader->mSizeExtra		= static_cast<zU32>(SizeTotal-inSize-sizeof(MallocHeader));
	pHeader->mItemCount		= static_cast<zU32>(inItemCount);
	AllocInfo.pMemory		= reinterpret_cast<zU8*>(&pHeader[1]);
	AllocInfo.Size			= pHeader->mSize + pHeader->mSizeExtra + sizeof(MallocHeader);
	return AllocInfo;
}

void PolicyNativeMalloc::Free(void* inpMemory)
{
	zenAssert(inpMemory);
	MallocHeader* pHeader = &reinterpret_cast<MallocHeader*>(inpMemory)[-1];
	zenAssert(pHeader->IsValid());
	free(pHeader);
}

SAllocInfo PolicyNativeMalloc::Resize(void* inpMemory, size_t inNewSize, size_t inItemCount)
{
	zenAssert(inpMemory);
	SAllocInfo AllocInfo;
	MallocHeader* pHeader	= &reinterpret_cast<MallocHeader*>(inpMemory)[-1];
	zenAssert(pHeader->IsValid()&& pHeader->mFlags.Any(zenMem::keFlag_Resize, zenMem::keFlag_ResizeLarge));	
	size_t TotalAvail		= pHeader->mSize + pHeader->mSizeExtra;
	bool bCanReuse			= (inNewSize <= TotalAvail) && (inNewSize >= TotalAvail/2);
	if( bCanReuse )
	{
		zI64 diff			= (zI64)inNewSize - (zI64)pHeader->mSize;
		pHeader->mSize		+= (zI16)diff;
		pHeader->mSizeExtra -= (zI16)diff;
		pHeader->mItemCount	= static_cast<zU32>(inItemCount);
		AllocInfo.pMemory	= reinterpret_cast<zU8*>(inpMemory);
		AllocInfo.Size		= pHeader->mSize + pHeader->mSizeExtra + sizeof(MallocHeader);
		return AllocInfo;
	}
	return Malloc(inNewSize, inItemCount, pHeader->mFlags);
}

size_t PolicyNativeMalloc::GetRequestedCount(void* _pMemory)
{
	const MallocHeader* pHeader = &reinterpret_cast<MallocHeader*>(_pMemory)[-1];
	zenAssert(pHeader->IsValid());
	return pHeader->mItemCount;
}

}  
