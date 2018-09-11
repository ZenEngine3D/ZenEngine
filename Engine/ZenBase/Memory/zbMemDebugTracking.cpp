#include "zbBase.h"

namespace zbMem
{
#if ZEN_MEMORY_TRACKING

DebugTracking::Footer::Footer()
{
	maFooter = {'A','l','l','o','c','a','t','i','o','n','F','o','o','t','e','r'};
}

DebugTracking::DebugTracking()
{
	zenStaticAssert(sizeof(Header) % kAlign == 0);	//Header must be a size multiple of alignment
	zenStaticAssert(sizeof(Footer) % kAlign == 0);	//Footer must be a size multiple of alignment	
}

#else

DebugTracking::DebugTracking()
{
}

#endif

}  

