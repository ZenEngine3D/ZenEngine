#include "zbBase.h"

namespace zbSys
{		
	static bool			sbSystemStarted(false);						//!< True if the system has been initialized
	static bool			sbEngineStarted(false);						//!< True if the engine's managers have been initialized
	static bool			(*spFuncEngineStart)(void)=nullptr;
	static bool			(*spFuncEngineStop)(void)=nullptr;
	
	//==================================================================================================
	//! @brief		Called at the program start, initializing the OS functionalities
	//! @brief		and the engine 
	//--------------------------------------------------------------------------------------------------
	//! @return		True if no error detected
	//==================================================================================================
	bool StartupSystem( bool (*_pFuncEngineStart)(), bool (*_pFuncEngineStop)() )
	{
		spFuncEngineStart	= _pFuncEngineStart;
		spFuncEngineStop	= _pFuncEngineStop;
		sbSystemStarted		= true;
		if( sbSystemStarted )
		{
			printf("\nSystem Initialization... Success");
			sbEngineStarted = spFuncEngineStart();
			if( !sbEngineStarted )
				spFuncEngineStop();
		}	
		else
			printf("\nSystem Initialization... Failed");
		
		if( sbSystemStarted && sbEngineStarted )
		{
			atexit( ShutdownSystem );
			return true;
		}
		return false;
	}

	//==================================================================================================
	//! @brief		Called at the program end, freeing up OS resources and shutting
	//! @brief		down the engine
	//--------------------------------------------------------------------------------------------------
	//! @return		True if no error detected
	//==================================================================================================
	void ShutdownSystem()
	{
		printf("\nShutdown... ");
		if( sbSystemStarted )
		{			
			if(sbEngineStarted)
			{				
				spFuncEngineStop();
				printf("\n  Engine unloaded");
			}			
			
			printf("\n  System unloaded\n");
			sbSystemStarted = false;
			sbEngineStarted = false;
		}
	}
	
	bool IsSystemActive()
	{
		return sbEngineStarted; //@todo gWindowsInfos.eMainWindowThreadStatus == keThread_Running && sbEngineStarted;
	}
}
