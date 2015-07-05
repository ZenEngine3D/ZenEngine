#include "zbBase.h"
#include <thread>

namespace zen { namespace zenSys 
{

zTask::zTask()
: mpOwnerFiber(NULL)
, muElapsed(0)
, muProfilingTimer(0)
, mbRunning(0)
, mGate(0)
, mpGateParent(NULL)
{
}

void zTask::AddToPending(std::atomic<zI32>* _pGateParent)
{
	mpGateParent = _pGateParent;
	if(mpGateParent)
		mpGateParent->fetch_add(1);
	
	zbSys::zbFiber::AddPendingTask(this);
}

void zTask::Suspend()
{
	mpOwnerFiber->Suspend();
}

void zTask::Cleanup()
{
	if(mpGateParent)
		mpGateParent->fetch_sub(1);
}

zUInt zTask::GetCPUCoreCount()
{
	return zenMath::Min<zUInt>(zbSys::zbFiber::keThreadMax, std::thread::hardware_concurrency());
}

void zTask::ProcessingStart()
{
	mbRunning.store(true);
	muProfilingTimer = zenSys::GetTimeUSec();
}

void zTask::ProcessingStop()
{
	mbRunning.store(false);
	muElapsed	+= zUInt(zenSys::GetTimeUSec() - muProfilingTimer);
}

void StartTaskProcessing()
{
	zbSys::StartTaskProcessing();
}



}}

