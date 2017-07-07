#pragma once

//=================================================================================================
// Main Class / functions
//=================================================================================================
namespace zbSys
{	
	bool StartupSystem( bool (*_pFuncEngineStart)(), bool (*_pFuncEngineStop)() );	
	void ShutdownSystem();
	bool IsSystemActive();	
}

#include zenHeaderPlatform( zbSysInit )
