#pragma once
#ifndef __LibZenBase_Api_Constant_Assert_h__
#define __LibZenBase_Api_Constant_Assert_h__

namespace zbLog 
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
#if ZEN_ASSERT_ON
	#if ZEN_ASSERT_BREAK_ON
		#define ASSERT_BREAKTYPE (zbLog::keErrorLevel_AssertBreak)
	#else
		#define ASSERT_BREAKTYPE (zbLog::keErrorLevel_AssertMsg)
	#endif		
	#define	ZENCriticalMsg(_Cond_, _Msg_, ...)		{   if( !(_Cond_) ) zbLog::Assert(#_Cond_, __FILE__, __LINE__, zbLog::keErrorLevel_Critical, _Msg_, __VA_ARGS__ ); }
	#define	ZENAssertMsg(_Cond_, _Msg_, ...)			{   if( !(_Cond_) ) zbLog::Assert(#_Cond_, __FILE__, __LINE__, ASSERT_BREAKTYPE, _Msg_, __VA_ARGS__ ); }	
	#define	ZENAssertMsgOnce(_Cond_, _Msg_, ...)		{   static bool sbTriggered=false;													\
														if( !sbTriggered && !(_Cond_) )													\
														{   sbTriggered= true;															\
															zbLog::Assert(#_Cond_, __FILE__, __LINE__, ASSERT_BREAKTYPE, _Msg_, __VA_ARGS__ );   }}
	#define	ZENWarningMsg(_Cond_, _Msg_, ...)		{   if( !(_Cond_) ) zbLog::Assert(#_Cond_, __FILE__, __LINE__, zbLog::keErrorLevel_Warning, _Msg_, __VA_ARGS__ ); }	
	#define	ZENWarningMsgOnce(_Cond_, _Msg_, ...)	{   static bool sbTriggered=false;													\
														if( !sbTriggered && !(_Cond_) )													\
														{   sbTriggered= true;															\
															zbLog::Assert(#_Cond_, __FILE__, __LINE__, zbLog::keErrorLevel_Warning, _Msg_, __VA_ARGS__ );   }}
	#define	ZENStaticAssertMsg(_Cond_, _Msg_, ...)	{	switch (0) {case 0: case (_Cond_):;} }

//=================================================================================================
// ASSERT Disabled
//=================================================================================================
#else	
	#define	ZENStaticAssertMsg(_Cond_, _Msg_, ...)	((void)0);
	#define ZENCriticalMsg(_Cond_, _Msg_, ...)		((void)0);
	#define	ZENAssertMsg(_Cond_, _Msg_, ...)		((void)0);
	#define	ZENAssertMsgOnce(_Cond_, _Msg_, ...)	((void)0);
	#define ZENWarningMsg(_Cond_, _Msg_, ...)		((void)0);
	#define	ZENWarningMsgOnce(_Cond_, _Msg_, ...)	((void)0);
	#define	ZENStaticAssertMsg(_Cond_, _Msg_, ...)	((void)0);

#endif

#define ZENStaticAssert(_Cond_)	ZENStaticAssertMsg(_Cond_, "")
#define	ZENCritical(_Cond_)		ZENCriticalMsg(_Cond_, "")
#define	ZENAssert(_Cond_)		ZENAssertMsg(_Cond_, "")
#define	ZENAssertOnce(_Cond_)	ZENAssertMsgOnce(_Cond_, "")
#define	ZENWarning(_Cond_)		ZENWarningMsg(_Cond_, "")
#define	ZENWarningOnce(_Cond_)	ZENWarningMsgOnce(_Cond_, "")

#endif