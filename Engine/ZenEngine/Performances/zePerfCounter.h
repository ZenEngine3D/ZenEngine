#pragma once
#ifndef __LibZenEngine_Perf_Counter_h__
#define __LibZenEngine_Perf_Counter_h__

namespace zePerf 
{

class zeCounterBase : public zRefCounted
{
ZENClassDeclare(zeCounterBase, zRefCounted)
public:
											zeCounterBase(const zStringHash32& _EventName);
	void virtual							Push() = 0;
	void virtual							Pop() = 0;
	
	
protected:
	
	zStringHash32							mzEventName		= zStringHash32("Unassigned");
	zU64									muTimeStart		= 0;
	zU64									muTimeElapsed	= 0;
};

}

#endif
