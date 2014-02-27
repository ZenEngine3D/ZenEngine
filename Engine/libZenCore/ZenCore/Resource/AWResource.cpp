#include "libZenCore.h"

namespace zen { namespace zenRes {	

awResourceRef::awResourceRef(awFlagResType _SupportedTypes, zenResID _ResourceID)
: mpResource(NULL)
{
	ZENDbgCode( mSupportedTypeMask = _SupportedTypes; )
	awResourceRef rResourceRef = EMgr::Resources.GetResource(_ResourceID); 
	SetResource( rResourceRef.mpResource );
}

const awResourceRef& awResourceRef::operator=(const zenResID& _ResourceID)
{
	awResourceRef rResourceRef = _ResourceID.IsValid() ? EMgr::Resources.GetResource(_ResourceID) : NULL;
	SetResource( rResourceRef.mpResource );
	return *this;
}

}} // namespace zen, zenRes


