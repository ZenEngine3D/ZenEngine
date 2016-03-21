#pragma once
#ifndef __LibZenEngine_Perf_CounterGPU_h__
#define __LibZenEngine_Perf_CounterGPU_h__

namespace zcPerf 
{

class zeEventGPU : public zeEventBase
{
ZENClassDeclare(zeEventGPU, zeEventBase)
public:									
	
	static zEngineRef<zeEventBase>	Create(const zStringHash32& _EventName);
	void virtual					Start();
	void virtual					Stop();

protected:
	void							StartCommon();
	void							StopCommon();
									zeEventGPU(const zStringHash32& _EventName) : Super(_EventName) {}
};

}

#endif
