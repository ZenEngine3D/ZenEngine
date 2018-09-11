#include "zcCore.h"

namespace zcPerf
{

zArrayDynamic< EventBaseRef > garStackEventCPU;

EventCPU_Base::EventCPU_Base(const zStringHash32& _EventName)
: Super(_EventName)
{
}

void EventCPU_Base::CPUStart()
{
	zenAssertMsg( muTimeStart==0, "Event can only be used once");
	mbActive = true;

	if (garStackEventCPU.IsEmpty() == false)
		(*garStackEventCPU.Last())->AddChild(*this);

	garStackEventCPU.Push(this);	
}

void EventCPU_Base::CPUStop()
{
	zenAssertMsg( IsActive(), "Start Event before ending it");
	zenAssertMsg((*garStackEventCPU.Last()).GetSafe() == this, "Stop events in the reverse order they were started");
	*garStackEventCPU.Last() = nullptr;
	garStackEventCPU.Pop();
	mbActive = false;
}

EventBaseRef EventCPU::Create(const zStringHash32& _EventName)
{
	//static zenMem::zAllocatorPool sMemPool("Pool Event CPU", sizeof(EventCPU), 256, 256);
	EventCPU* pEventCpu = zenNewPool EventCPU(_EventName);
	return pEventCpu;
}

}