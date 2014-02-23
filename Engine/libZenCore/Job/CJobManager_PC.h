#pragma once
#ifndef __LibCore_Task_Manager_PC_h__
#define __LibCore_Task_Manager_PC_h__

namespace CJob
{
	class ManagerJob : public ManagerJob_Base
	{
	AWClassDeclare(ManagerJob, ManagerJob_Base);
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