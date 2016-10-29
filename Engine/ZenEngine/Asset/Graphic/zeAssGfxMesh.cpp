#include "zeEngine.h"

#if ZEN_ENGINETOOL

namespace zeAss
{

const zenAss::PropertyDefArray& GfxMesh::GetProperties()const
{ 	
	//! @todo Asset : TODO TODO
	static zenAss::PropertyDefRef arPropertiesAll[] = {
		zenAss::PropertyBool::Create("TestBool",		"", "Test Bool Field",		true,	false, true),
	};
	static zenAss::PropertyDefArray sarProperties( arPropertiesAll, zenArrayCount(arPropertiesAll) );
	return sarProperties;	
}

} //namespace zeAss

#else

zenPreventEmptyFileWarning

#endif //ZEN_ENGINETOOL
