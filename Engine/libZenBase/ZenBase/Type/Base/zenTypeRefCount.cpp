#include <libZenBase.h>

namespace zen { namespace zenType {

zRefCounted::~zRefCounted()
{
}

void zRefCounted::ReferenceNoneCB()
{
}
	
void zRefCountedAutoDel::ReferenceNoneCB()
{
	delete this;
}

}} // namespace zen, zenType
