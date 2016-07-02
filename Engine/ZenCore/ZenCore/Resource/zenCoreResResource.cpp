#include "zcCore.h"

namespace zen { namespace zenRes {	

zArrayDynamic<zResource*> zResource::saPendingDelete[2];
zU8 zResource::suPendingDeleteIndex = 0;

void zResource::ReleaseUnused()
{	
	zU8 uPrevIndex			= suPendingDeleteIndex;
	suPendingDeleteIndex	= (suPendingDeleteIndex + 1) & 0x01;
	saPendingDelete[suPendingDeleteIndex].Clear();
	saPendingDelete[suPendingDeleteIndex].Reserve(256);

	for(zUInt idx(0), count(saPendingDelete[uPrevIndex].Count()); idx<count; ++idx)
	{
		zResource* pResToDel			= saPendingDelete[uPrevIndex][idx];
		pResToDel->mbIsPendingDelete	= false;
		if( pResToDel->miRefCount <= 0 )
			zenDel(pResToDel);
	}	
}

void zResource::ReferenceNoneCB()
{

	if( !mbIsPendingDelete)
	{
		mbIsPendingDelete = true;
		saPendingDelete[suPendingDeleteIndex].Push(this);
	}	
}

zResource::~zResource()
{
	if( mResID.GetSource() == zenConst::keResSource_Runtime )
		zcDepot::ResourceData.ClearItem(mResID);
}

zResourceRef::zResourceRef(zFlagResType _SupportedTypes, zResID _ResourceID)
: mpResource(nullptr)
{
	ZENDbgCode( mSupportedTypeMask = _SupportedTypes; )
	const zResourceRef& rResourceRef = zcDepot::Resources.GetResource(_ResourceID); 
	SetResource( rResourceRef.mpResource );
}

const zResourceRef& zResourceRef::operator=(const zResID& _ResourceID)
{
	const zResourceRef& rResourceRef = _ResourceID.IsValid() ? zcDepot::Resources.GetResource(_ResourceID) : nullptr;
	SetResource( rResourceRef.mpResource );
	return *this;
}

zResourceRef::~zResourceRef()
{
	SetResource(nullptr);
}


}} // namespace zen, zenRes


