#include "libZenCore.h"

namespace CJob
{

void ManagerJob_Base::Add(JobItem* _pTask)
{
	_pTask->meStatus	= JobItem::keStatus_Wait;
	_pTask->mTimeStart	= zenSys::GetTimeStamp();
	mlstTasksPending.AddTail(_pTask);	
}

void ManagerJob_Base::Update()
{
	// Move each ready pending task, into readylist
	zList2xNode* pItem = mlstTasksPending.GetHead();
	while( pItem != mlstTasksPending.GetInvalid() )
	{
		JobItem* pTask = static_cast<JobItem*>(pItem);
		if( !pTask->Start() )
		{
			pTask->meStatus = JobItem::keStatus_FailStart;
			Remove(pTask);
		}
		else if( pTask->meStatus == JobItem::keStatus_Ready )	
			mlstTasksReady.AddTail(pTask);
	}

	// Remove every completed task
	pItem = mlstTasksRunning.GetHead();
	while( pItem != mlstTasksRunning.GetInvalid() )
	{
		JobItem* pTask = static_cast<JobItem*>(pItem);
		pItem		= pItem->LstNext();
		if( pTask->meStatus != JobItem::keStatus_Running )
			Remove(pTask);
	}

	// Move every task started by child thread, into running list
	pItem = mlstTasksReady.GetHead();
	while( pItem != mlstTasksReady.GetInvalid() )
	{
		JobItem* pTask = static_cast<JobItem*>(pItem);
		pItem		= pItem->LstNext();
		if( pTask->meStatus != JobItem::keStatus_Wait )
			mlstTasksRunning.AddTail(pTask);
	}
}

void ManagerJob_Base::Remove(JobItem* _pTask)
{	
	// Send Notify of the end of the task	
	bool bSuccess = _pTask->End();
	if( !_pTask->IsFailed() && !bSuccess )
		_pTask->meStatus = JobItem::keStatus_FailEnd;

	if( _pTask->mbAutoCleanup )
		_pTask->Cleanup();
}

zList2x& ManagerJob_Base::GetTaskReady()
{
	return mlstTasksReady;
}

}  
