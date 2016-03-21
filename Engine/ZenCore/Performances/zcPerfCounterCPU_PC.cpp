#include "zcCore.h"

namespace zcPerf
{

void zeEventCPU::Start() 
{
	StartCommon();
	muTimeStart = zenSys::GetTimeUSec();
}

void zeEventCPU::Stop() 
{
	StopCommon();	
	muTimeElapsed	= zenSys::GetTimeUSec() - muTimeStart;
}

}