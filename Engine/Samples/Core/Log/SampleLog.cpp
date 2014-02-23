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
	CMgr::Log.SetActive(CLog::keLogOut_StdOut, CLog::keLog_Assert );
	CMgr::Log.SetActive(CLog::keLogOut_StdOut, CLog::keLog_FileIO );
	CMgr::Log.SetActive(CLog::keLogOut_Screen, CLog::keLog_Assert );
	CMgr::Log.SetActive(CLog::keLogOut_Screen, CLog::keLog_FileIO );
	CMgr::Log.SetActive(CLog::keLogOut_Screen, CLog::keLog_Assert, false );
	CMgr::Log.SetActive(CLog::keLogOut_StdOut, CLog::keLog_Game );

	bool bVal1 = CMgr::Log.IsActive(CLog::keLogOut_StdOut, CLog::keLog_Assert);
	bool bVal2 = CMgr::Log.IsActive(CLog::keLogOut_Screen, CLog::keLog_Assert);

	CMgr::Log.Log(CLog::keLog_ResTexture, "Test des erreurs" );
	CMgr::Log.Log(CLog::keLog_Game, "Printf test %i %i", 1, 2);
}

}
