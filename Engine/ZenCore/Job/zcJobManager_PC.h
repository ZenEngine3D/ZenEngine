#pragma once
#ifndef __zCore_Job_Manager_PC_h__
#define __zCore_Job_Manager_PC_h__

namespace zcJob
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