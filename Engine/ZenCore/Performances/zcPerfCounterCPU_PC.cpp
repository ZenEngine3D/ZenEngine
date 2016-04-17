#include "zcCore.h"

namespace zcPerf
{

EventCPU_PC::EventCPU_PC(const zStringHash32& _EventName)
: Super(_EventName)
{
}

void EventCPU_PC::Start() 
{
	Super::Start();
	muTimeStart = zenSys::GetTimeUSec();
}

void EventCPU_PC::Stop() 
{
	Super::Stop();
	muTimeElapsed = zenSys::GetTimeUSec() - muTimeStart;
}

}