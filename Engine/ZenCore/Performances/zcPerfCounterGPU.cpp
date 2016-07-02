#include "zcCore.h"

namespace zcPerf
{

zArrayDynamic< zEngineRef<EventBase> > garStackEventGPU;
static const zStringHash32 gaGPUCounterName[] = {
	zStringHash32("Cmd Draw Indexed"),	//keType_DrawIndexed
	zStringHash32("Cmd ClearColor"),	//keType_ClearColor
	zStringHash32("Cmd ClearDepth"),	//keType_ClearDepth	
	zStringHash32("Cmd Compute"),		//keType_Compute
	zStringHash32("Cmd Invalid"),		//keType__Count
};

//=================================================================================================
// EventGPU Base
//=================================================================================================
EventGPU_Base::EventGPU_Base(const zStringHash32& _EventName) 
: Super(_EventName) 
{
}

void EventGPU_Base::Start()
{
	ZENAssertMsg( muTimeStart==0, "Event can only be used once");
	muTimeStart		= zenSys::GetTimeUSec();
	mbActive		= true;

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

//=================================================================================================
// EventGPU
//=================================================================================================
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

//=================================================================================================
// EventGPU Counter
//=================================================================================================
zEngineRef<EventBase> EventGPUCounter::Create(eType _eCounterType)
{
	ZENStaticAssert( ZENArrayCount(gaGPUCounterName)==EventGPUCounter::keType__Count+1 ); //Make sure Counter description array match enum
	static zenMem::zAllocatorPool sMemPool("Pool Event GPU Counter", sizeof(EventGPUCounter), 256, 256);
	EventGPUCounter* pEventGpuCounter = zenNew(&sMemPool) EventGPUCounter(_eCounterType);
	pEventGpuCounter->Start();
	return pEventGpuCounter;
}

EventGPUCounter::EventGPUCounter(eType _eCounterType)
: Super(gaGPUCounterName[_eCounterType])
, meCounterType(_eCounterType)
{
	ZENAssert(_eCounterType != keType__Invalid);
}

void EventGPUCounter::Start()
{
	Super::Start();
	Super::Stop();
}


}
