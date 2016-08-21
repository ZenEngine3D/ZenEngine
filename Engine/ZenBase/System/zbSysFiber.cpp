#include "zbBase.h"

namespace zbSys
{

zThreadsafeQueueFifo<zenSys::zTask*, 1024>				gQueuePendingTask;
zThreadsafeQueueFifo<zbFiber*, zbFiber::keFiberCount>	gQueueAvailableFiber;
zThreadsafeQueueFifo<zbFiber*, zbFiber::keFiberCount>	gQueueSuspendedFiber[zbFiber::keThreadMax];

void FiberUpdateLoop(zUInt _uThreadId)
{
	while( 1 )
	{
		zenSys::zTask* pNewTask = nullptr;
		zbFiber* pFiber	= nullptr;
		zbFiber* pNotReadyFiber[zbFiber::keFiberCount];
		zUInt uNotReadyCount(0);

		// Loop all suspended task until none can currently be resumed
		while( gQueueSuspendedFiber[_uThreadId].Pop(pFiber) )
		{
			if( pFiber->CanResume() )
				pFiber->Activate(_uThreadId);				
			else
				pNotReadyFiber[uNotReadyCount++] = pFiber;
		}

		// Re-add the suspended fibers to the pending list
		for(zUInt idx(0); idx<uNotReadyCount; ++idx)
			gQueueSuspendedFiber[_uThreadId].Push(pNotReadyFiber[idx]);

		// Take on a new task waiting to be run for 1st time
		if( gQueuePendingTask.Pop(pNewTask) )
		{
			bool bReady		= pNewTask->CanResume();
			bool bAvailable = gQueueAvailableFiber.Pop(pFiber);
			if( bReady && bAvailable )
			{								
				pFiber->Activate(_uThreadId, pNewTask);
			}
			// Unable to run at the moment, push back to pending queue
			else
			{
				if( bReady ) 
					gQueuePendingTask.Push(pNewTask);
				if( bAvailable )
					gQueueAvailableFiber.Push(pFiber);
			}
		}
	}
}

void zbFiber::InfiniteLoop()
{	
	while( 1 )
	{
		if( mpRunningTask )
		{
			mpRunningTask->Run();
			mpRunningTask->Cleanup();
		}		
		Deactivate();
	}
}

zbFiber::zbFiber(zUInt _StackSize)
: zbFiber_PC(_StackSize)
{
	AddFreeFiber(this);
}

void zbFiber::AddPendingTask(zenSys::zTask* _pTask)
{
	gQueuePendingTask.Push(_pTask);
}

void zbFiber::AddFreeFiber(zbFiber* _pFreeFiber)
{
	zenAssert(_pFreeFiber);
	gQueueAvailableFiber.Push(_pFreeFiber);
}

void zbFiber::Activate(zUInt _uThreadId, zenSys::zTask* _pTask)
{
	mpRunningTask				= _pTask != nullptr ? + _pTask : mpRunningTask;
	muThreadId					= _uThreadId; 
	zenAssert(mpRunningTask);
	mpRunningTask->mpOwnerFiber = this;
	mpRunningTask->ProcessingStart();
	HALActivate();
}

void zbFiber::Deactivate()
{
	zenAssert(mpRunningTask);
	mpRunningTask->ProcessingStop();
	mpRunningTask = nullptr;	
	gQueueAvailableFiber.Push(this);
	HALDeactivate();
}

void zbFiber::Suspend()
{
	zenAssert(mpRunningTask);	
	mpRunningTask->ProcessingStop();	
	gQueueSuspendedFiber[muThreadId].Push(this);
	HALDeactivate();
}

}
