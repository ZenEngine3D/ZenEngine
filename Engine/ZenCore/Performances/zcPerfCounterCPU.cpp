#include "zcCore.h"

namespace zcPerf
{

zArrayDynamic< zEngineRef<zeEventBase> > garStackEventCPU;

zGameRef<zeEventBase> zeEventCPU::Create(const zStringHash32& _EventName)
{
	static zenMem::zAllocatorPool sMemPool("Pool Event CPU", sizeof(zeEventCPU), 256, 256);
	zeEventCPU* pEventCpu = zenNew(&sMemPool) zeEventCPU(_EventName);
	return pEventCpu;
}

void zeEventCPU::StartCommon()
{
	ZENAssertMsg(muTimeStart == 0, "Event can only be used once");

	if (garStackEventCPU.IsEmpty() == false)
		(*garStackEventCPU.Last())->AddChild(*this);

	garStackEventCPU.Push(this);
}

void zeEventCPU::StopCommon()
{
	ZENAssertMsg(muTimeStart != 0, "Start Event before ending it");
	ZENAssertMsg((*garStackEventCPU.Last()).GetSafe() == this, "Stop events in the reverse order they were started");
	garStackEventCPU.Pop();
}

}