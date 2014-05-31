#pragma once
#ifndef __LibZenBase_Task_Manager_h__
#define __LibZenBase_Task_Manager_h__

//! @todo Clean: rename namespace
namespace CJob
{

class ManagerJob_Base : public zbType::Manager
{
ZENClassDeclare(ManagerJob_Base, zbType::Manager);
public:
	void		Add(JobItem* _pJob);
	void		Update();
	zList2x&	GetTaskReady();

protected:
	void		Remove(JobItem* _pJob);
	zList2x	mlstTasksPending;
	zList2x	mlstTasksReady;
	zList2x	mlstTasksRunning;
};

}  

#include ZENHeaderPlatform( CJobManager )

namespace zcMgr 
{ 
	extern CJob::ManagerJob Job; 
}

#endif