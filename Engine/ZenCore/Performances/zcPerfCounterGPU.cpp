#include "zcCore.h"

namespace zcPerf
{

zArrayDynamic< zEngineRef<zeEventBase> > garStackEventGPU;

zEngineRef<zeEventBase> zeEventGPU::Create(const zStringHash32& _EventName)
{
	static zenMem::zAllocatorPool sMemPool("Pool Event GPU", sizeof(zeEventCPU), 256, 256);
	zeEventGPU* pEventCpu = zenNew(&sMemPool) zeEventGPU(_EventName);
	return pEventCpu;
}

void zeEventGPU::StartCommon()
{
}

void zeEventGPU::StopCommon()
{
}

}