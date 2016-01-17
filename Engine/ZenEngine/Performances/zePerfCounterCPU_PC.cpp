#include "zeEngine.h"

namespace zePerf
{
	zeCounterCPU gCounterCPURoot("Root CPU Counter");
	zeCounterCPU* gpTopCounterCPU = &gCounterCPURoot;

	void zeCounterCPU::Push() 
	{
		ZENAssertMsg(muTimeStart==0, "Counter can only be used once");
		muTimeStart		= zenSys::GetTimeStamp();
		mpParent		= gpTopCounterCPU;
		gpTopCounterCPU = this;
		mpParent->mlstChilds.PushTail(*this);		
	}

	void zeCounterCPU::Pop() 
	{
		muTimeElapsed	= zenSys::GetTimeStamp() - muTimeStart;
		gpTopCounterCPU = mpParent;
	}
}