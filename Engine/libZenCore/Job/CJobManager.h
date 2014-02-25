#pragma once
#ifndef __LibZenBase_Task_Manager_h__
#define __LibZenBase_Task_Manager_h__

namespace CJob
{

class ManagerJob_Base : public zbType::Manager
{
AWClassDeclare(ManagerJob_Base, zbType::Manager);
public:
	void		Add(JobItem* _pJob);
	void		Update();
	zenList2x&	GetTaskReady();

protected:
	void		Remove(JobItem* _pJob);
	zenList2x	mlstTasksPending;
	zenList2x	mlstTasksReady;
	zenList2x	mlstTasksRunning;
};

}  

#include AWHeaderPlatform( CJobManager )

namespace CMgr { extern CJob::ManagerJob Job; }

#endif