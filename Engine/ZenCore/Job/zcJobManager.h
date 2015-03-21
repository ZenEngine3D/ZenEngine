#pragma once
#ifndef __zCore_Job_Manager_h__
#define __zCore_Job_Manager_h__

//! @todo Clean: rename namespace
namespace zcJob
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

#include ZENHeaderPlatform( zcJobManager )

namespace zcMgr 
{ 
	extern zcJob::ManagerJob Job; 
}

#endif