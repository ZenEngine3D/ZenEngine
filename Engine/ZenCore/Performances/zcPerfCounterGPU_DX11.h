#pragma once
#ifndef __LibZenEngine_Perf_CounterGPU_DX11_h__
#define __LibZenEngine_Perf_CounterGPU_DX11_h__

namespace zcGfx { class DX11QueryTimestamp; } //forward declare

namespace zcPerf 
{

class EventGPU_DX11 : public EventGPU_Base
{
ZENClassDeclare(EventGPU_DX11, EventGPU_Base)
public:									
	virtual void							Start();
	virtual void							Stop();
	virtual double							GetElapsedMs();

protected:
											EventGPU_DX11(const zStringHash32& _EventName);
	zEngineRef<zcGfx::DX11QueryTimestamp>	mrQueryTimestampStart;
	zEngineRef<zcGfx::DX11QueryTimestamp>	mrQueryTimestampStop;
};

typedef EventGPU_DX11 EventGPU_Platform ;

}

#endif
