#pragma once

namespace zcPerf{ class EventBase; }

namespace zen { namespace zenPerf 
{

class zEventRef : public zEngineRef<zcPerf::EventBase>
{
zenClassDeclare(zEventRef, zEngineRef);
public:
	static zEventRef		CreateCPU(const zStringHash32& _EventName);
	void					Start();
	void					Stop();

	zEventRef				GetFirstChild()const;
	zEventRef				GetNext() const;
	zEventRef				GetPrev() const;
	const zStringHash32&	GetName() const;
	using zEngineRef<zcPerf::EventBase>::operator=;
	using zEngineRef<zcPerf::EventBase>::zEngineRef;
};

class zScopedEventCpu
{
public:
	zScopedEventCpu(const zStringHash32& _EventName);
	~zScopedEventCpu();
protected:
	zEventRef mrEvent;
};

}}
