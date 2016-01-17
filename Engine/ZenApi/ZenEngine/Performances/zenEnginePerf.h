#pragma once
#ifndef __LibZenEngine_Perf_h__
#define __LibZenEngine_Perf_h__

namespace zePerf{ class zeCounterBase; }

namespace zen { namespace zenAss 
{

class zPerfCounterRef : public zEngineRef<zePerf::zeCounterBase>
{
ZENClassDeclare(zPerfCounterRef, zEngineRef);
public:
	static zPerfCounterRef	PushCPU(const zStringHash32& _EventName);
	static zPerfCounterRef	PushGPU(const zStringHash32& _EventName);
	void Pop();

protected:
							zPerfCounterRef(const zPerfCounterRef& _Copy);
							zPerfCounterRef(zePerf::zeCounterBase* _pCounter);
	using zEngineRef<zePerf::zeCounterBase>::operator=;
};

}}
#endif
