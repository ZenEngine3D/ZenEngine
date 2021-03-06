namespace zbLog
{
	//==================================================================================================
	//! @brief		Create a header for each Log line
	//! @details	This insert info about time and LogType
	//--------------------------------------------------------------------------------------------------
	//! @param[in]	_eLogType - Type of log to generate
	//! @return[in]	zString describing Log being outputted, valid until method called again
	//==================================================================================================
	const char* ManagerLog::GetTypeStamp( zenConst::eLogType _eLogType )
	{
		static char sBufferStamp[32];
		const zDateTime& oDate = zenSys::GetDateTime();
		::sprintf_s(sBufferStamp, sizeof(sBufferStamp), "[%02ih%02i:%02i %15s] ", oDate.muHour, oDate.muMinute, oDate.muSecond, ssLogTypeDesc[_eLogType] );
		return sBufferStamp;
	}

	//==================================================================================================
	//! @brief		Return the right  number of spaces to match length of TypeStamp
	//! @details	Use this to write on multiple line while remaining aligned to stamp
	//--------------------------------------------------------------------------------------------------
	//! @return		zString of spaces matching GetTypeStamp length
	//==================================================================================================
	const char* ManagerLog::GetEmptyStamp()
	{
		static char* sBufferStamp="                           ";
		return sBufferStamp;
	}

	void ManagerLog::SetActive( eLogOutput _eOutput, zenConst::eLogType _eType, bool _bActive )
	{ 
		zenAssertMsg( _eOutput < keLogOut__Count, "Invalid output");
		zU32 uMask = (1<<_eType);
		muOutputMask[_eOutput] &= ~uMask;
		muOutputMask[_eOutput] |= (zU32)_bActive>0 ? uMask : 0;
	}

	void ManagerLog::Clear( eLogOutput _eOutput )
	{
		zenAssertMsg( _eOutput < keLogOut__Count, "Invalid output");
		muOutputMask[_eOutput] = 0;
	}

	bool ManagerLog::IsActive( eLogOutput _eOutput, zenConst::eLogType _eType )
	{
		zenAssertMsg( _eOutput < keLogOut__Count, "Invalid output");
		return (muOutputMask[_eOutput] & (1<<_eType)) != 0;
	}

	//==================================================================================================
	//! @brief		Similar to Printf with a Timestamp and LogType description added
	//! @details	Final Log method expects a va_list, this creates it.
	//--------------------------------------------------------------------------------------------------
	//! @param[in]	_eLogType - Type of log to generate
	//! @param[in]	_zFormat - Regular printf format specifier
	//==================================================================================================
	void ManagerLog::Log( zenConst::eLogType _eLogType, const char* _zFormat, ...)
	{	
		va_list pArgList;
		va_start(pArgList, _zFormat);
		Log(_eLogType, _zFormat, pArgList);
		va_end(pArgList);
	}

	//==================================================================================================
	//! @brief		Prepare text with variable argument list to be sent to Printf
	//! @details	Final printf method expects a va_list, this creates it.
	//--------------------------------------------------------------------------------------------------
	//! @param[in]	_eLogType - Type of log to generate
	//! @param[in]	_zFormat - Regular printf format specifier
	//==================================================================================================
	void ManagerLog::Printf( zenConst::eLogType _eLogType, const char* _zFormat, ...)
	{	
		va_list pArgList;
		va_start(pArgList, _zFormat);
		Printf( _eLogType, _zFormat, pArgList);
		va_end(pArgList);
	}
}   