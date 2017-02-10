#pragma once
#ifndef __zBase_System_Init_h__ 
#define __zBase_System_Init_h__

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

#endif
