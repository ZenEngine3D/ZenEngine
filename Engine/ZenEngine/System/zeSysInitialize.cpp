#include "zeEngine.h"

namespace FSys
{
//==================================================================================================
//! @brief		Called at the program start, initializing the OS functionalities
//! @details	Take care to load managers in order
//--------------------------------------------------------------------------------------------------
//! @return		True if no error detected
//==================================================================================================
bool StartupEngine()
{
	bool bResult(true);

	bResult &= zbMgr::Log.Startup();
	bResult &= zbMgr::File.Startup();
	bResult &= EMgr::Export.Startup();
	bResult	&= zcDepot::Resources.Startup();
	bResult &= zcDepot::ResourceData.Startup();	
	bResult &= EMgr::GfxState.Startup();
	bResult &= EMgr::GfxRender.Startup();
#if ZEN_ENGINETOOL
	bResult &= zeMgr::Asset.Startup();
#endif
	if( bResult )	printf("\nEngine Initialization... Success");
	else			printf("\nEngine Initialization... Failed");
	return bResult;
}

//==================================================================================================
//! @brief		Called at the program end, shutting down the engine
//! @details	Take care to reverse unload the managers
//--------------------------------------------------------------------------------------------------
//! @return		bool True if no error detected
//==================================================================================================
bool ShutdownEngine()
{
	bool bResult(true);	
#if ZEN_ENGINETOOL
	bResult &= zeMgr::Asset.Shutdown();
#endif
	bResult &= EMgr::GfxRender.Shutdown();
	bResult &= EMgr::GfxState.Shutdown();		
	bResult &= zcDepot::ResourceData.Shutdown();
	bResult &= zcDepot::Resources.Shutdown();
	bResult &= EMgr::Export.Shutdown();
	bResult &= zbMgr::File.Shutdown();
	bResult &= zbMgr::Log.Shutdown();

	return bResult;
}

//==================================================================================================
//! @brief		Ask the core system to initialized itself
//! @details	Need pointer to function initializing each manager in the proper order
//!	@details	and unloading at the end of the program
//--------------------------------------------------------------------------------------------------
//! @return		bool True if no error detected
//==================================================================================================
bool EngineStart()
{
	return zbSys::StartupSystem(StartupEngine, ShutdownEngine);
}

//==================================================================================================
//! @brief		Ask the engine to shutdown
//--------------------------------------------------------------------------------------------------
//! @return		bool True if no error detected
//==================================================================================================
void EngineStop()
{	
	zbSys::ShutdownSystem();
}

}
