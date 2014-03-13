#include "libZenCore.h"

namespace zen { namespace zenRes {	

zResourceRef::zResourceRef(zenFlagResType _SupportedTypes, zResID _ResourceID)
: mpResource(NULL)
{
	ZENDbgCode( mSupportedTypeMask = _SupportedTypes; )
	zResourceRef rResourceRef = EMgr::Resources.GetResource(_ResourceID); 
	SetResource( rResourceRef.mpResource );
}

const zResourceRef& zResourceRef::operator=(const zResID& _ResourceID)
{
	zResourceRef rResourceRef = _ResourceID.IsValid() ? EMgr::Resources.GetResource(_ResourceID) : NULL;
	SetResource( rResourceRef.mpResource );
	return *this;
}

}} // namespace zen, zenRes


