#include "zeEngine.h"

namespace zen {	namespace zenAss
{

zPerfCounterRef zPerfCounterRef::PushCPU(const zStringHash32& _EventName)
{
	zPerfCounterRef rCounter = zenNewDefault zePerf::zeCounterCPU(_EventName);
	rCounter->Push();
	return rCounter;	
}

zPerfCounterRef zPerfCounterRef::PushGPU(const zStringHash32& _EventName)
{
	zPerfCounterRef rCounter = zenNewDefault zePerf::zeCounterGPU(_EventName);
	rCounter->Push();
	return rCounter;
}

zPerfCounterRef::zPerfCounterRef(const zPerfCounterRef& _Copy)
: Super(_Copy)
{
}

zPerfCounterRef::zPerfCounterRef(zePerf::zeCounterBase* _pCounter)
: Super(_pCounter)
{
}

void zPerfCounterRef::Pop()
{
	GetSafe()->Pop();
}



}}