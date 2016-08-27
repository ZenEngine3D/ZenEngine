#pragma once
#ifndef __LibZenEngine_Perf_h__
#define __LibZenEngine_Perf_h__

namespace zcPerf{ class EventBase; }

namespace zen { namespace zenPerf 
{

class zEventRef : public zEngineRef<zcPerf::EventBase>
{
zenClassDeclare(zEventRef, zEngineRef);
public:
	static zEventRef		CreateCPU(const zStringHash32& _EventName);
	static zEventRef		CreateGPU(const zStringHash32& _EventName);
	void					Start();
	void					Stop();
	
	zEventRef				GetFirstChild()const;
	zEventRef				GetNext() const;
	zEventRef				GetPrev() const;
	const zStringHash32&	GetName() const;
	using zEngineRef<zcPerf::EventBase>::operator=;
protected:
					//	zEventRef(const zEventRef& _Copy);
					//	zEventRef(zcPerf::zeCounterBase* _pCounter);	
					//void operator=(zEngineRef<zcPerf::zeEventBase>);
};

class zScopedEventCpu
{
public:
	zScopedEventCpu(const zStringHash32& _EventName);
	~zScopedEventCpu();
protected:
	zEventRef mrEvent;
};

class zScopedEventGpu
{
public:
	zScopedEventGpu(const zStringHash32& _EventName);
	~zScopedEventGpu();
protected:
	zEventRef mrEvent;
};

}}
#endif
