#include "zenEngine.h"

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

//	zbMgr::Log.Reset();
	zenIO::Log(zenConst::keLog_Game, zenConst::kzLineA40);
	zenIO::Log(zenConst::keLog_Game, " Assert test");
	zenIO::Log(zenConst::keLog_Game, zenConst::kzLineA40);
	
	zenWarning(bTestWarning);
	zenWarningMsg(bTestWarningMsg, "Warning with message" );
	for(int i=0; i<3; ++i)
	{
		zenWarningOnce(bTestWarningOnce);
		zenWarningMsgOnce(bTestWarningOnceMsg, "Warning Once with message");
	}

	zenAssert(bTestAssert);
	zenAssertMsg(bTestAssertMsg, "Assert with message" );
	for(int i=0; i<3; ++i)
	{
		zenAssertOnce(bTestAssertOnce);
		zenAssertMsgOnce(bTestAssertOnceMsg, "Assert Once with message");
	}

	zenCriticalMsg(bTestCriticalMsg, "Critical, termine program");
}

}
