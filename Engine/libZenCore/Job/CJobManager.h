#pragma once
#ifndef __LibCore_Task_Manager_h__
#define __LibCore_Task_Manager_h__

namespace CJob
{

class ManagerJob_Base : public zbType::Manager
{
AWClassDeclare(ManagerJob_Base, zbType::Manager);
public:
	void		Add(JobItem* _pJob);
	void		Update();
	awList2x&	GetTaskReady();

protected:
	void		Remove(JobItem* _pJob);
	awList2x	mlstTasksPending;
	awList2x	mlstTasksReady;
	awList2x	mlstTasksRunning;
};

}  

#include AWHeaderPlatform( CJobManager )

namespace CMgr { extern CJob::ManagerJob Job; }

#endif