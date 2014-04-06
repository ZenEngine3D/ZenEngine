#include "libZenBase.h"
#include <stdio.h>
#include <assert.h>

namespace zbLog
{
	//==================================================================================================
	//! @brief		Reset the output associated with each eLogType to default values
	//==================================================================================================
	void ManagerLog::Reset()
	{
		for(int iOut=0; iOut<keLogOut__Count; ++iOut)
			Clear((eLogOutput)iOut);
		SetActive(keLogOut_StdOut, keLog_General );
		SetActive(keLogOut_StdOut, keLog_Game );
		SetActive(keLogOut_StdOut, keLog_Critical );
		SetActive(keLogOut_StdOut, keLog_Assert );
		SetActive(keLogOut_StdOut, keLog_Warning );
		SetActive(keLogOut_StdOut, keLog_FileIO );
		SetActive(keLogOut_StdOut, keLog_ResTexture );
		SetActive(keLogOut_StdOut, keLog_ResMaterial );
		SetActive(keLogOut_StdOut, keLog_ResGeometry );
		SetActive(keLogOut_StdOut, keLog_DebugInfo );
		SetActive(keLogOut_StdOut, keLog_ExportError);
		SetActive(keLogOut_StdOut, keLog_ExportSuccess);
	}

	//==================================================================================================
	//! @brief		Similar to Printf with a Timestamp and LogType description added
	//--------------------------------------------------------------------------------------------------
	//! @param[in]	_eLogType - Type of log to generate
	//! @param[in]	_zFormat - Regular printf format specifier
	//! @param[in]	_pArgs - Variable argument list
	//==================================================================================================
	void ManagerLog::Log( eLogType _eLogType, const char* _zFormat, const va_list _pArgs )
	{	
		char sBufferText[256];
		::sprintf_s(sBufferText, sizeof(sBufferText), "\n%s%s",  GetTypeStamp(_eLogType), _zFormat);
		Printf( _eLogType, sBufferText, _pArgs);
	}

	//==================================================================================================
	//! @brief		Final method called to display text of the various output
	//! @details	Based on the LogType, the string will be written to various 
	//!	@n			outputs supported by the platform (stdout, File, Tcp/Ip, ...)
	//! @n			and associated in the manager
	//--------------------------------------------------------------------------------------------------
	//! @param[in]	_eLogType - Type of log to generate
	//! @param[in]	_zFormat - Regular printf format specifier
	//! @param[in]	_pArgs - List of arguments to use with asFormat string
	//==================================================================================================
	void ManagerLog::Printf( eLogType _eLogType, const char* _zFormat, const va_list _pArgs )
	{	
		char sBufferText[1024];
		::vsprintf_s(sBufferText, sizeof(sBufferText), _zFormat, _pArgs);
		
		//-----------------------------------------------------
		// StdOut output
		//-----------------------------------------------------
		if( muOutputMask[keLogOut_StdOut] & _eLogType )
		{		
			::printf(sBufferText);
		}
		//-----------------------------------------------------
		// Screen output
		//-----------------------------------------------------
		if( muOutputMask[keLogOut_Screen] & _eLogType )
		{
		}
		//-----------------------------------------------------
		// LogFile output
		//-----------------------------------------------------
		if( muOutputMask[keLogOut_StdOut] & _eLogType )
		{		
		}
	}
}   
