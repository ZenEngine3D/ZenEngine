#pragma once

namespace zen 
{ 

namespace zenConst
{

enum eLogType
{
	keLog_General,
	keLog_Game,
	keLog_Critical,		
	keLog_Assert,
	keLog_Warning,
	keLog_FileIO,
	keLog_ExportError,
	keLog_ExportSuccess,
	keLog_ResShader,
	keLog_ResTexture,
	keLog_ResMaterial,
	keLog_ResGeometry,
	keLog_DebugInfo,
	keLog__Count,
	keLog__None,
};

}

namespace zenIO
{

void Log	( zenConst::eLogType _eLogType, const char* _zFormat, ... );
void Log	( zenConst::eLogType _eLogType, const char* _zFormat, const va_list _pArgs );		
void Printf	( zenConst::eLogType _eLogType, const char* _zFormat, ... );		
void Printf	( zenConst::eLogType _eLogType, const char* _zFormat, const va_list _pArgs );
	
}

} // namespace zen { namespace zenIO
