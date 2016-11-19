#pragma once
#ifndef __LibZenEngine_Perf_CounterGPU_DX12_h__
#define __LibZenEngine_Perf_CounterGPU_DX12_h__
//SF DX12

namespace zcGfx { class DX12QueryTimestamp; } //forward declare

namespace zcPerf 
{

class EventGPU_DX12 : public EventGPU_Base
{
zenClassDeclare(EventGPU_DX12, EventGPU_Base)
public:									
	virtual void							Start();
	virtual void							Stop();
	virtual double							GetElapsedMs();

protected:
											EventGPU_DX12(const zStringHash32& _EventName);
	zEngineRef<zcGfx::DX12QueryTimestamp>	mrQueryTimestampStart;
	zEngineRef<zcGfx::DX12QueryTimestamp>	mrQueryTimestampStop;
};

typedef EventGPU_DX12 EventGPU_Platform ;

}

#endif
