#pragma once
#ifndef __LibZenEngine_Perf_CounterCPU_h__
#define __LibZenEngine_Perf_CounterCPU_h__

namespace zcPerf 
{

class zeEventCPU : public zeEventBase
{
ZENClassDeclare(zeEventCPU, zeEventBase)
public:						
	void virtual					Start();
	void virtual					Stop();	
	static zGameRef<zeEventBase>	Create(const zStringHash32& _EventName); //! @todo Urgent, until we expose child list properly

protected:
	void							StartCommon();
	void							StopCommon();
									zeEventCPU(const zStringHash32& _EventName): Super(_EventName) {}
};

}

#endif
