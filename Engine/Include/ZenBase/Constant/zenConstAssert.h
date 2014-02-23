#pragma once
#ifndef __LibCore_Api_Constant_Assert_h__
#define __LibCore_Api_Constant_Assert_h__

namespace CLog 
{
//=================================================================================================
//! @brief Level of the error detected
//=================================================================================================
	enum eErrorLevel
	{
		keErrorLevel_Warning,
		keErrorLevel_AssertMsg,
		keErrorLevel_AssertBreak,
		keErrorLevel_Critical
	};

	void Assert( const char* _zCond, const char* _zFilename, unsigned int _uLine, eErrorLevel _uErrorType, const char* _zMessage, ... );
}  



//=================================================================================================
// ASSERT Enabled or ASSERT Message only
//=================================================================================================
#if AW_ASSERTON
	#if AW_ASSERT_BREAKON
		#define ASSERT_BREAKTYPE (CLog::keErrorLevel_AssertBreak)
	#else
		#define ASSERT_BREAKTYPE (CLog::keErrorLevel_AssertMsg)
	#endif		
	#define	AWCriticalMsg(_Cond_, _Msg_, ...)		{   if( !(_Cond_) ) CLog::Assert(#_Cond_, __FILE__, __LINE__, CLog::keErrorLevel_Critical, _Msg_, __VA_ARGS__ ); }
	#define	AWAssertMsg(_Cond_, _Msg_, ...)			{   if( !(_Cond_) ) CLog::Assert(#_Cond_, __FILE__, __LINE__, ASSERT_BREAKTYPE, _Msg_, __VA_ARGS__ ); }	
	#define	AWAssertMsgOnce(_Cond_, _Msg_, ...)		{   static bool sbTriggered=false;													\
														if( !sbTriggered && !(_Cond_) )													\
														{   sbTriggered= true;															\
															CLog::Assert(#_Cond_, __FILE__, __LINE__, ASSERT_BREAKTYPE, _Msg_, __VA_ARGS__ );   }}
	#define	AWWarningMsg(_Cond_, _Msg_, ...)		{   if( !(_Cond_) ) CLog::Assert(#_Cond_, __FILE__, __LINE__, CLog::keErrorLevel_Warning, _Msg_, __VA_ARGS__ ); }	
	#define	AWWarningMsgOnce(_Cond_, _Msg_, ...)	{   static bool sbTriggered=false;													\
														if( !sbTriggered && !(_Cond_) )													\
														{   sbTriggered= true;															\
															CLog::Assert(#_Cond_, __FILE__, __LINE__, CLog::keErrorLevel_Warning, _Msg_, __VA_ARGS__ );   }}
	#define	AWStaticAssertMsg(_Cond_, _Msg_, ...)	{	switch (0) {case 0: case (_Cond_):;} }

//=================================================================================================
// ASSERT Disabled
//=================================================================================================
#else	
	#define	AWStaticAssertMsg(_Cond_, _Msg_, ...)	((void)0);
	#define AWCriticalMsg(_Cond_, _Msg_, ...)		((void)0);
	#define	AWAssertMsg(_Cond_, _Msg_, ...)			((void)0);
	#define	AWAssertMsgOnce(_Cond_, _Msg_, ...)		((void)0);
	#define AWWarningMsg(_Cond_, _Msg_, ...)		((void)0);
	#define	AWWarningMsgOnce(_Cond_, _Msg_, ...)	((void)0);
	#define	AWStaticAssertMsg(_Cond_, _Msg_, ...)	((void)0);

#endif

#define AWStaticAssert(_Cond_)	AWStaticAssertMsg(_Cond_, "")
#define	AWCritical(_Cond_)		AWCriticalMsg(_Cond_, "")
#define	AWAssert(_Cond_)		AWAssertMsg(_Cond_, "")
#define	AWAssertOnce(_Cond_)	AWAssertMsgOnce(_Cond_, "")
#define	AWWarning(_Cond_)		AWWarningMsg(_Cond_, "")
#define	AWWarningOnce(_Cond_)	AWWarningMsgOnce(_Cond_, "")

#endif