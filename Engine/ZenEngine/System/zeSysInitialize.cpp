#include "zeEngine.h"

namespace zbSys
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
	bResult &= zcMgr::Export.Startup();
	bResult &= zcDepot::ExportData.Startup();	
	bResult &= zcMgr::GfxRender.Startup();
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
	bResult &= zcMgr::GfxRender.Shutdown();
	bResult &= zcDepot::ExportData.Shutdown();
	bResult &= zcMgr::Export.Shutdown();
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
