#pragma once
#ifndef __LibZenBase_CLogManager_h__
#define __LibZenBase_CLogManager_h__

#include <stdio.h>
#include <stdarg.h>

namespace CLog
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
AWClassDeclare(ManagerLog, zbType::Manager);
public:  	
			void			Reset			();
	inline	void			Clear			( eLogOutput _eOutput );
	inline	void			SetActive		( eLogOutput _eOutput, eLogType _eType, bool _bActive=true );
	inline	bool			IsActive		( eLogOutput _eOutput, eLogType _eType );
				
	inline	void			Log				( eLogType _eErrorType, const char* _sFormat, ... );
			void			Log				( eLogType _eErrorType, const char* _sFormat, const va_list _pArgs );		
	inline	void			Printf			( eLogType _eErrorType, const char* _sFormat, ... );		
			void			Printf			( eLogType _eErrorType, const char* _sFormat, const va_list _pArgs );				
		
	inline	const char*		GetTypeStamp	( eLogType _eLogType );	
	inline  const char*		GetEmptyStamp	();

protected:
	virtual	bool			Load			();
	virtual	bool			Unload			();	
	zenU32						muOutputMask[keLogOut__Count];	//!< List if supported error type for each output
	static	const char*		ssLogTypeDesc[keLog__Count+1];	//!< Description of each error type
};

//inline void Printf(eLogType _eType, const char* _zText, ...);
//inline void Log(eLogType _eType, const char* _zFormat, ... );

}   

namespace CMgr { extern CLog::ManagerLog Log; }

#include "CLogMgr.inl"


#endif
