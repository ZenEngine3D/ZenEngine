#pragma once
#ifndef __LibZenBase_Log_Manager_h__
#define __LibZenBase_Log_Manager_h__

#include <stdio.h>
#include <stdarg.h>

namespace zbLog
{

//=================================================================================================
//! @brief Types of log, which dictate the output(s) used to display the text
//!	@note	Do not forget to update ssLogTypeDesc when adding/removing items 
//=================================================================================================
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

//=================================================================================================
//! @brief Possible outputs the user can associate with each eLogType
//=================================================================================================
enum eLogOutput
{
	keLogOut_StdOut,
	keLogOut_Screen,
	keLogOut_Logfile,
	keLogOut__Count
};

//=================================================================================================
//! @class		ManagerLog
//-------------------------------------------------------------------------------------------------
//! @brief		Write text to various output.
//! @details	Keep track of which output to use for each eLogType, format the text and write to various supported output.
//=================================================================================================
class ManagerLog : public zbType::Manager
{
ZENClassDeclare(ManagerLog, zbType::Manager);
public:  	
				void			Reset			();
	ZENInline	void			Clear			( eLogOutput _eOutput );
	ZENInline	void			SetActive		( eLogOutput _eOutput, eLogType _eType, bool _bActive=true );
	ZENInline	bool			IsActive		( eLogOutput _eOutput, eLogType _eType );
				
	ZENInline	void			Log				( eLogType _eErrorType, const char* _sFormat, ... );
				void			Log				( eLogType _eErrorType, const char* _sFormat, const va_list _pArgs );		
	ZENInline	void			Printf			( eLogType _eErrorType, const char* _sFormat, ... );		
				void			Printf			( eLogType _eErrorType, const char* _sFormat, const va_list _pArgs );				
		
	ZENInline	const char*		GetTypeStamp	( eLogType _eLogType );	
	ZENInline	const char*		GetEmptyStamp	();

protected:
	virtual	bool				Load			();
	virtual	bool				Unload			();	
	zU32						muOutputMask[keLogOut__Count];	//!< List if supported error type for each output
	static	const char*			ssLogTypeDesc[keLog__Count+1];	//!< Description of each error type
};

//ZENInline void Printf(eLogType _eType, const char* _zText, ...);
//ZENInline void Log(eLogType _eType, const char* _zFormat, ... );

}   

namespace zbMgr { extern zbLog::ManagerLog Log; }

#include "zbLogMgr.inl"


#endif
