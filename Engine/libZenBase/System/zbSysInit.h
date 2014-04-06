#pragma once
#ifndef __LibZenBase_System_Init_h__ 
#define __LibZenBase_System_Init_h__

//=================================================================================================
// Main Class / functions
//=================================================================================================
namespace zbSys
{	
	bool StartupSystem( bool (*_pFuncEngineStart)(), bool (*_pFuncEngineStop)() );	
	void ShutdownSystem();
	bool IsSystemActive();	
}  

#include ZENHeaderPlatform( zbSysInit )

#endif
