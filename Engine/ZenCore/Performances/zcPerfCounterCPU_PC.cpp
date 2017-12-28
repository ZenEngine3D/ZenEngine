#include "zcCore.h"

namespace zcPerf
{

EventCPU_PC::EventCPU_PC(const zStringHash32& _EventName)
: Super(_EventName)
{
}

void EventCPU_PC::Start() 
{
	Super::CPUStart();
	muTimeStart = zenSys::GetTimeUSec();
}

void EventCPU_PC::Stop() 
{
	Super::CPUStop();
	muTimeElapsed = zenSys::GetTimeUSec() - muTimeStart;
}

}