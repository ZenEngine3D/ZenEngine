#pragma once
#ifndef __LibCor_System_Init_h__
#define __LibCor_System_Init_h__

//=================================================================================================
// Main Class / functions
//=================================================================================================
namespace CSys
{	
	bool StartupSystem( bool (*_pFuncEngineStart)(), bool (*_pFuncEngineStop)() );	
	void ShutdownSystem();
	bool IsSystemActive();	
}  

#include AWHeaderPlatform( zbSysInit )

#endif
