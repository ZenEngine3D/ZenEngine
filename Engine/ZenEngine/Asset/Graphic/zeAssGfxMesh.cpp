#include "zeEngine.h"

#if ZEN_ENGINETOOL

namespace zen { namespace zeAss
{

const zenAss::PropertyDefArray& GfxMesh::GetProperties()const
{ 	
	//! @todo Asset : TODO TODO
	static zenAss::PropertyDefRef arPropertiesAll[] = {
		zenAss::PropertyBool::Create("TestBool",		"", "Test Bool Field",		true,	false, true),
	};
	static zenAss::PropertyDefArray sarProperties( arPropertiesAll, ZENArrayCount(arPropertiesAll) );
	return sarProperties;	
}

}} //namespace zen { namespace zeAss

#endif //ZEN_ENGINETOOL
