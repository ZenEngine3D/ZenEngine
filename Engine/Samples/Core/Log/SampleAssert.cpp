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
	CMgr::Log.Log(CLog::keLog_Game, awconst::kzLineA40);
	CMgr::Log.Log(CLog::keLog_Game, " Assert test");
	CMgr::Log.Log(CLog::keLog_Game, awconst::kzLineA40);
	
	AWWarning(bTestWarning);
	AWWarningMsg(bTestWarningMsg, "Warning with message" );
	for(int i=0; i<3; ++i)
	{
		AWWarningOnce(bTestWarningOnce);
		AWWarningMsgOnce(bTestWarningOnceMsg, "Warning Once with message");
	}

	AWAssert(bTestAssert);
	AWAssertMsg(bTestAssertMsg, "Assert with message" );
	for(int i=0; i<3; ++i)
	{
		AWAssertOnce(bTestAssertOnce);
		AWAssertMsgOnce(bTestAssertOnceMsg, "Assert Once with message");
	}

	AWCriticalMsg(bTestCriticalMsg, "Critical, termine program");
}

}
