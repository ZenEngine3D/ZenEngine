#include "libZenEngine.h"
#include <iostream>

//=================================================================================================
//! @example SampleLog.cpp
//! Test of message logging system
//=================================================================================================

namespace sample
{
void SampleLog()
{	
	CMgr::Log.SetActive(zbLog::keLogOut_StdOut, zbLog::keLog_Assert );
	CMgr::Log.SetActive(zbLog::keLogOut_StdOut, zbLog::keLog_FileIO );
	CMgr::Log.SetActive(zbLog::keLogOut_Screen, zbLog::keLog_Assert );
	CMgr::Log.SetActive(zbLog::keLogOut_Screen, zbLog::keLog_FileIO );
	CMgr::Log.SetActive(zbLog::keLogOut_Screen, zbLog::keLog_Assert, false );
	CMgr::Log.SetActive(zbLog::keLogOut_StdOut, zbLog::keLog_Game );

	bool bVal1 = CMgr::Log.IsActive(zbLog::keLogOut_StdOut, zbLog::keLog_Assert);
	bool bVal2 = CMgr::Log.IsActive(zbLog::keLogOut_Screen, zbLog::keLog_Assert);

	CMgr::Log.Log(zbLog::keLog_ResTexture, "Test des erreurs" );
	CMgr::Log.Log(zbLog::keLog_Game, "Printf test %i %i", 1, 2);
}

}
