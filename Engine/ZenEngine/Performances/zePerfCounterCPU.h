#pragma once
#ifndef __LibZenEngine_Perf_CounterCPU_h__
#define __LibZenEngine_Perf_CounterCPU_h__

namespace zePerf 
{

class zeCounterCPU : public zeCounterBase
{
ZENClassDeclare(zeCounterCPU, zeCounterBase)
public:
						zeCounterCPU(const zStringHash32& _EventName):Super(_EventName){}
	void virtual		Push();
	void virtual		Pop();	

protected:	
	zeCounterCPU*									mpParent = nullptr;
	zListLink										mlnkChilds;		
	zList<zeCounterCPU, &zeCounterCPU::mlnkChilds>	mlstChilds; //!< All children to this Counter	
};

}

#endif
