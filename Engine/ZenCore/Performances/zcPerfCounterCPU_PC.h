#pragma once
#ifndef __LibZenEngine_Perf_CounterCPU_PC_h__
#define __LibZenEngine_Perf_CounterCPU_PC_h__

namespace zcPerf 
{

class EventCPU_PC : public EventCPU_Base
{
zenClassDeclare(EventCPU_PC, EventCPU_Base)
public:									
	virtual void							Start();
	virtual void							Stop();

protected:
											EventCPU_PC(const zStringHash32& _EventName);
};

typedef EventCPU_PC EventCPU_Platform;

}

#endif
