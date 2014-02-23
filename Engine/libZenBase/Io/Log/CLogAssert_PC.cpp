#include "libZenBase.h"
#include <stdlib.h>
#include <crtdbg.h>
#include <intrin.h>

namespace CLog
{

//==================================================================================================
//! @brief		Called when the code has asserted, display message and break execution if requested
//--------------------------------------------------------------------------------------------------
//! @param[in]	_zCond - Description of the test that failed
//! @param[in]	_zFilename - Filename where the error occurred
//! @param[in]	_uLine - Line where the error occurred
//! @param[in]	_eErrorType - Change the reaction of the system according to gravity of error
//! @param[in]	_zMessage - Additional message to display along basic informations
//==================================================================================================
void Assert( const char* _zCond, const char* _zFilename, unsigned int _uLine, eErrorLevel _eErrorType, const char* _zMessage, ... )
{
	eLogType eLog(keLog_Assert);
	if( _eErrorType == keErrorLevel_Critical )		eLog = keLog_Critical;
	else if( _eErrorType == keErrorLevel_Warning )	eLog = keLog_Warning;

	va_list pArgList;
	va_start(pArgList, _zMessage);
	char Buffer[1024];
	sprintf_s(Buffer, "\n%s Infos     : %s",	CMgr::Log.GetEmptyStamp(), _zMessage);
	

	CMgr::Log.Printf(eLog, "\n%s Condition : %s",		CMgr::Log.GetTypeStamp(eLog), _zCond);
	CMgr::Log.Printf(eLog, "\n%s Location  : %s:%i",	CMgr::Log.GetEmptyStamp(), _zFilename, _uLine);
	//CMgr::Log.Printf(eLog, "\n%s Infos     : %s",		CMgr::Log.GetEmptyStamp(), _zMessage);
	CMgr::Log.Printf(eLog, Buffer, pArgList);
	va_end(pArgList);

	if( _eErrorType == keErrorLevel_AssertBreak )
	{
		#ifdef _DEBUG
			switch (_CrtDbgReport(_CRT_ERROR, NULL, 0, NULL, "(Abort) Stop\n(Retry) Break\n(Ignore) Continue"))
			{
			case 2: exit(-1);
			case 1: __debugbreak();
			}
		#else
			__debugbreak();
		#endif
	}
	else if( _eErrorType == keErrorLevel_Critical )
	{	
		__debugbreak();
		exit(-1);
	}
	
}

} 