#include "libZenCore.h"

namespace CMgr { CJob::ManagerJob Job; }

namespace CJob
{

DWORD WINAPI ExecuteThreadTask( LPVOID lpParam )
{
	while(1)
	{
		awList2xNode* pItem = CMgr::Job.GetTaskReady().GetHead();
		while( pItem != CMgr::Job.GetTaskReady().GetInvalid() )
		{
			JobItem* pTask = static_cast<JobItem*>(pItem);
			pItem = pItem->LstNext();
			if( pTask->meStatus == JobItem::keStatus_Ready )
			{
				pTask->meStatus		= JobItem::keStatus_Running;
				pTask->mTimeStart	= zenSys::GetTimeStamp();
				pTask->meStatus		= pTask->THRTask_Execute() ? JobItem::keStatus_Success : JobItem::keStatus_FailExe;
				pTask->mTimeStop	= zenSys::GetTimeStamp();
			}			
		}
		SleepEx(256, false);
	}
}

bool ManagerJob::Load()
{
	mhExecuteTaskThread = CreateThread( NULL, 0,  ExecuteThreadTask, NULL, 0, NULL );  
	return mhExecuteTaskThread != NULL;
}

bool ManagerJob::Unload()
{
	SuspendThread(mhExecuteTaskThread);
	CloseHandle(mhExecuteTaskThread);
	return true;
}
	
}  

