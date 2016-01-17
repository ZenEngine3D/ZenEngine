#pragma once
#ifndef __zenBase_Type_Manager_h__
#define __zenBase_Type_Manager_h__

namespace zbType
{
//=================================================================================================
//! @class		ManagerBase
//-------------------------------------------------------------------------------------------------
//! @brief		Base class used by all manager for tracking their single instance
//! @details	Make sure each manager has been started and stopped
//=================================================================================================
class Manager
{
ZENClassDeclareNoParent(Manager);
public:
	//=============================================================================================
	//! @brief		Should be empty
	//! @details	Since we can't insure the initialization order of manager, constructor 
	//!				should only initialize its member to nullptr value, and do its proper initialization
	//!				inside Load()	
	//=============================================================================================
	Manager()
	: mbIsloaded(false)
	{
	}

	//=============================================================================================
	//! @brief		Should be empty
	//! @details	Since we can't insure the destruction order of manager, destructor 
	//!				do nothing, and rely on Unload() for the release of resources
	//=============================================================================================
	virtual ~Manager()
	{
		ZENCriticalMsg(!mbIsloaded, "Stop the manager before engine termination");
	}	
	
	//=============================================================================================
	//! @brief		Entry point to start a manager
	//! @details	At engine init, all managers should be initialized in order they need 
	//!				to be loaded.
	//!--------------------------------------------------------------------------------------------
	//! @return		True if init was successful
	//=============================================================================================
	bool Startup()
	{
		ZENCriticalMsg(!mbIsloaded, "This manager has already been started");	
		mbIsloaded = Load();
		return mbIsloaded;
	}

	//=============================================================================================
	//! @brief		Entry point to stop a manager
	//! @details	At engine unloading, all managers should be unloaded in proper order
	//!--------------------------------------------------------------------------------------------
	//! @return		True if init was successful
	//=============================================================================================
	bool Shutdown()
	{
		ZENCriticalMsg(mbIsloaded, "This manager has already been stopped");	
		mbIsloaded = Unload();
		return !mbIsloaded;
	}

protected:
	//==================================================================================================
	//! @brief		Load
	//! @details	Called when engine first started. Child class managers 
	//!				should initialize their resources here.
	//! @note		Child classes must have init code here
	//!-----------------------------------------------------------------------------
	//! @return		True if init was successful
	//==================================================================================================
	virtual	bool Load()=0;		
	
	//==================================================================================================
	//! @brief		Unload
	//! @details	Called when engine is about to unload. Child class managers
	//!				should release all resources here.
	//! @note		Child classes must have un-init code here
	//!-----------------------------------------------------------------------------
	//! @return		True if init was successful
	//==================================================================================================
	virtual bool Unload()=0;		

private:							
	bool	mbIsloaded;
};


}  

#endif
