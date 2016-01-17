#pragma once
#ifndef __LibZenEngine_Perf_CounterGPU_h__
#define __LibZenEngine_Perf_CounterGPU_h__

namespace zePerf 
{

class zeCounterGPU : public zeCounterBase
{
ZENClassDeclare(zeCounterGPU, zeCounterBase)
public:
					zeCounterGPU(const zStringHash32& _EventName) : Super(_EventName) {}
	void virtual	Push() {};
	void virtual	Pop() {};
};

}

#endif
