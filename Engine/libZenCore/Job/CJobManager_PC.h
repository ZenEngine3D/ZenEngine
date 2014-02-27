#pragma once
#ifndef __LibZenBase_Task_Manager_PC_h__
#define __LibZenBase_Task_Manager_PC_h__

namespace CJob
{
	class ManagerJob : public ManagerJob_Base
	{
	ZENClassDeclare(ManagerJob, ManagerJob_Base);
	public:
		
	protected:
		HANDLE				mhExecuteTaskThread;			//!< Handle on execution task thread
	//---------------------------------------------------------
	// ManagerBase Section
	//---------------------------------------------------------
	public:
		virtual	bool		Load();
		virtual	bool		Unload();
	};

}  

#endif