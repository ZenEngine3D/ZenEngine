#include "zcCore.h"

namespace zcPerf
{

zArrayDyn< EventBaseRef > garStackEventCPU;

EventCPU_Base::EventCPU_Base(const zStringHash32& _EventName)
: Super(_EventName)
{
}

void EventCPU_Base::CPUStart()
{
	zenAssertMsg( muTimeStart==0, "Event can only be used once");
	mbActive = true;
	if (garStackEventCPU.empty() == false)
		garStackEventCPU.back()->AddChild(*this);

	garStackEventCPU.push_back(this);	
}

void EventCPU_Base::CPUStop()
{
	zenAssertMsg( IsActive(), "Start Event before ending it");
	zenAssertMsg(garStackEventCPU.back().GetSafe() == this, "Stop events in the reverse order they were started");
	garStackEventCPU.pop_back();
	mbActive = false;
}

EventBaseRef EventCPU::Create(const zStringHash32& _EventName)
{
	EventCPU* pEventCpu = zenMem::NewPool<EventCPU>(_EventName);
	return pEventCpu;
}

}