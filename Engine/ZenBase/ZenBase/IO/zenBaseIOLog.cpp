#include "zbBase.h"

namespace zen { namespace zenIO
{   

void Log( zenConst::eLogType _eLogType, const char* _zFormat, ... )
{
	va_list pArgList;
	va_start(pArgList, _zFormat);
	zbMgr::Log.Log(_eLogType, _zFormat, pArgList);
	va_end(pArgList);
}

void Log( zenConst::eLogType _eLogType, const char* _zFormat, const va_list _pArgs )
{
	zbMgr::Log.Log(_eLogType, _zFormat, _pArgs);
}

void Printf( zenConst::eLogType _eLogType, const char* _zFormat, ... )
{
	va_list pArgList;
	va_start(pArgList, _zFormat);
	zbMgr::Log.Printf(_eLogType, _zFormat, pArgList);
	va_end(pArgList);	
}

void Printf( zenConst::eLogType _eLogType, const char* _zFormat, const va_list _pArgs )
{
	zbMgr::Log.Printf(_eLogType, _zFormat, _pArgs);
}

}} //namespace zen { namespace zenIO
