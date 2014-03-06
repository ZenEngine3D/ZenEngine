#include "libZenEngine.h"

//=================================================================================================
//! @example SampleAssert.cpp
//! Test of errors logging system
//=================================================================================================

namespace sample
{
void SampleAssert()
{
	bool bTestWarning(false);	bool bTestWarningMsg(false);	bool bTestWarningOnce(false);	bool bTestWarningOnceMsg(false);
	bool bTestAssert(false);	bool bTestAssertMsg(false);		bool bTestAssertOnce(false);	bool bTestAssertOnceMsg(false);
	bool bTestCriticalMsg(false);

	CMgr::Log.Reset();
	CMgr::Log.Log(CLog::keLog_Game, zenConst::kzLineA40);
	CMgr::Log.Log(CLog::keLog_Game, " Assert test");
	CMgr::Log.Log(CLog::keLog_Game, zenConst::kzLineA40);
	
	ZENWarning(bTestWarning);
	ZENWarningMsg(bTestWarningMsg, "Warning with message" );
	for(int i=0; i<3; ++i)
	{
		ZENWarningOnce(bTestWarningOnce);
		ZENWarningMsgOnce(bTestWarningOnceMsg, "Warning Once with message");
	}

	ZENAssert(bTestAssert);
	ZENAssertMsg(bTestAssertMsg, "Assert with message" );
	for(int i=0; i<3; ++i)
	{
		ZENAssertOnce(bTestAssertOnce);
		ZENAssertMsgOnce(bTestAssertOnceMsg, "Assert Once with message");
	}

	ZENCriticalMsg(bTestCriticalMsg, "Critical, termine program");
}

}
