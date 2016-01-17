#include "zbBase.h"

namespace zbSys
{

zbFiber_Base::zbFiber_Base(zUInt _uStackSize)
: mpRunningTask(nullptr)
, muThreadId(0)
, muStackSize(_uStackSize)
{
}

/*
void zbFiber_Base::UpdateLoop()
{
	while( 1 )
	{
		if( mpRunningTask )
		{
			mpRunningTask->muProfilingTimer = zenSys::GetTimeUSec();
			mpRunningTask->Run();
			mpRunningTask->Cleanup();
		}		
		Deactivate();
	}
}

void zbFiber_Base::Deactivate()
{
	ZENAssert(mpRunningTask);
	mpRunningTask->muElapsed	+= zUInt(zenSys::GetTimeUSec() - mpRunningTask->muProfilingTimer);
	mpRunningTask				= nullptr; //! @todo cleanup fiber Free data ?
	gQueueAvailableFiber.Push(this);
}

void zbFiber_Base::Suspend()
{
	ZENAssert(mpRunningTask);
	mpRunningTask->muElapsed	+= zUInt(zenSys::GetTimeUSec() - mpRunningTask->muProfilingTimer);
	gQueueSuspendedFiber[muThreadId].Push(this);
}
*/


}
