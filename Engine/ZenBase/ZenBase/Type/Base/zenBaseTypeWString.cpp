#include "zbBase.h"

namespace zen { namespace zenType {

void zWString::Replace(wchar_t* _zSource, wchar_t* _zReplace, zUInt _uPos)
{
	size_t uSrcLen = wcslen(_zSource);
	size_t uRplLen = wcslen(_zReplace);
	zenAssertMsg(uSrcLen==uRplLen, "Only supports same lenght string at the moment, implement different lenght when needed.");
	std::string::size_type idx = find (_zSource, _uPos);
	while( idx != std::string::npos )
	{
		replace(idx,uSrcLen,_zReplace);
		idx = find (_zSource, _uPos);
	}
}

}}  //namespace zen, Type 
