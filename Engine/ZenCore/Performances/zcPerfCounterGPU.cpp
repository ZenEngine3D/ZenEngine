#include "zcCore.h"

namespace zcPerf
{

zArrayDynamic< zEngineRef<EventBase> > garStackEventGPU;

EventGPU_Base::EventGPU_Base(const zStringHash32& _EventName) 
: Super(_EventName) 
{
}

void EventGPU_Base::Start()
{
	ZENAssertMsg( muTimeStart==0, "Event can only be used once");
	muTimeStart = zenSys::GetTimeUSec();
	mbActive	= true;

	if (garStackEventGPU.IsEmpty() == false)
		(*garStackEventGPU.Last())->AddChild(*this);

	garStackEventGPU.Push(this);
}

void EventGPU_Base::Stop()
{
	ZENAssertMsg( IsActive(), "Start Event before ending it");
	ZENAssertMsg((*garStackEventGPU.Last()).GetSafe() == this, "Stop events in the reverse order they were started");
	mbActive = false;	
	garStackEventGPU.Pop();	
}

zEngineRef<EventBase> EventGPU::Create(const zStringHash32& _EventName)
{
	static zenMem::zAllocatorPool sMemPool("Pool Event GPU", sizeof(EventGPU), 256, 256);
	EventGPU* pEventGpu = zenNew(&sMemPool) EventGPU(_EventName);
	return pEventGpu;
}

EventGPU::EventGPU(const zStringHash32& _EventName) 
: Super(_EventName) 
{
}

}
