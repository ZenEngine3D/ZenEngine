#pragma once

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
	#define	zenCriticalMsg(_Cond_, _Msg_, ...)		{   if( !(_Cond_) ) zbLog::Assert(#_Cond_, __FILE__, __LINE__, zbLog::keErrorLevel_Critical, _Msg_, __VA_ARGS__ ); }
	#define	zenAssertMsg(_Cond_, _Msg_, ...)		{   if( !(_Cond_) ) zbLog::Assert(#_Cond_, __FILE__, __LINE__, ASSERT_BREAKTYPE, _Msg_, __VA_ARGS__ ); }	
	#define	zenAssertMsgOnce(_Cond_, _Msg_, ...)	{   static bool sbTriggered=false;													\
														if( !sbTriggered && !(_Cond_) )													\
														{   sbTriggered= true;															\
															zbLog::Assert(#_Cond_, __FILE__, __LINE__, ASSERT_BREAKTYPE, _Msg_, __VA_ARGS__ );   }}
	#define	zenWarningMsg(_Cond_, _Msg_, ...)		{   if( !(_Cond_) ) zbLog::Assert(#_Cond_, __FILE__, __LINE__, zbLog::keErrorLevel_Warning, _Msg_, __VA_ARGS__ ); }
	#define	zenWarningMsgOnce(_Cond_, _Msg_, ...)	{   static bool sbTriggered=false;													\
														if( !sbTriggered && !(_Cond_) )													\
														{   sbTriggered= true;															\
															zbLog::Assert(#_Cond_, __FILE__, __LINE__, zbLog::keErrorLevel_Warning, _Msg_, __VA_ARGS__ );   }}
	#define	zenStaticAssertMsg(_Cond_, _Msg_)		{	static_assert(_Cond_, _Msg_);}

//=================================================================================================
// ASSERT Disabled
//=================================================================================================
#else	
	#define	zenStaticAssertMsg(_Cond_, _Msg_, ...)	((void)0);
	#define zenCriticalMsg(_Cond_, _Msg_, ...)		((void)0);
	#define	zenAssertMsg(_Cond_, _Msg_, ...)		((void)0);
	#define	zenAssertMsgOnce(_Cond_, _Msg_, ...)	((void)0);
	#define zenWarningMsg(_Cond_, _Msg_, ...)		((void)0);
	#define	zenWarningMsgOnce(_Cond_, _Msg_, ...)	((void)0);
	#define	zenStaticAssertMsg(_Cond_, _Msg_, ...)	((void)0);

#endif

//! @todo clean replace macro with 'static_assert' and use zenAssert
#define zenStaticAssert(_Cond_)			zenStaticAssertMsg(_Cond_, #_Cond_)
#define	zenCritical(_Cond_)				zenCriticalMsg(_Cond_, "")
#define	zenAssert(_Cond_)				zenAssertMsg(_Cond_, "")
#define	zenAssertOnce(_Cond_)			zenAssertMsgOnce(_Cond_, "")
#define	zenWarning(_Cond_)				zenWarningMsg(_Cond_, "")
#define	zenWarningOnce(_Cond_)			zenWarningMsgOnce(_Cond_, "")
