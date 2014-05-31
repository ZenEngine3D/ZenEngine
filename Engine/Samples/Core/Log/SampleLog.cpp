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
	zbMgr::Log.SetActive(zbLog::keLogOut_StdOut, zbLog::keLog_Assert );
	zbMgr::Log.SetActive(zbLog::keLogOut_StdOut, zbLog::keLog_FileIO );
	zbMgr::Log.SetActive(zbLog::keLogOut_Screen, zbLog::keLog_Assert );
	zbMgr::Log.SetActive(zbLog::keLogOut_Screen, zbLog::keLog_FileIO );
	zbMgr::Log.SetActive(zbLog::keLogOut_Screen, zbLog::keLog_Assert, false );
	zbMgr::Log.SetActive(zbLog::keLogOut_StdOut, zbLog::keLog_Game );

	bool bVal1 = zbMgr::Log.IsActive(zbLog::keLogOut_StdOut, zbLog::keLog_Assert);
	bool bVal2 = zbMgr::Log.IsActive(zbLog::keLogOut_Screen, zbLog::keLog_Assert);

	zbMgr::Log.Log(zbLog::keLog_ResTexture, "Test des erreurs" );
	zbMgr::Log.Log(zbLog::keLog_Game, "Printf test %i %i", 1, 2);
}

}
