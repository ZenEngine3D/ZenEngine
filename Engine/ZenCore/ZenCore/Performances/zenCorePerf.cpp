#include "zcCore.h"

namespace zen {	namespace zenPerf
{

zEventRef zEventRef::CreateCPU(const zStringHash32& _EventName)
{
	zEventRef rEvent;
	rEvent = zcPerf::zeEventCPU::Create(_EventName).Get();
	return rEvent;
}

zEventRef zEventRef::CreateGPU(const zStringHash32& _EventName)
{
	zEventRef rEvent;
	rEvent = zcPerf::zeEventGPU::Create(_EventName).Get();
	return rEvent;
}

void zEventRef::Start()
{
	GetSafe()->Start();
}

void zEventRef::Stop()
{
	GetSafe()->Stop();
}

zEventRef zEventRef::GetFirstChild()const
{
	zEventRef rEvent;
	rEvent = GetSafe()->mlstChilds.GetHead();
	return rEvent;
}

zEventRef zEventRef::GetNext() const
{
	//! @todo optim : add support copy constructor with pointer
	zEventRef rEvent;
	rEvent = zcPerf::zeEventBase::TypeListChild::GetNext(*GetSafe());
	return rEvent;
}

zEventRef zEventRef::GetPrev() const
{
	zEventRef rEvent;
	rEvent = zcPerf::zeEventBase::TypeListChild::GetPrev(*GetSafe());
	return rEvent;
}

const zStringHash32&	zEventRef::GetName() const
{
	return GetSafe()->mzEventName;
}

zScopedEventCpu::zScopedEventCpu(const zStringHash32& _EventName)
{
	mrEvent = zEventRef::CreateCPU(_EventName);
	mrEvent.Start();
}

zScopedEventCpu::~zScopedEventCpu()
{
	mrEvent.Stop();
}

zScopedEventGpu::zScopedEventGpu(const zStringHash32& _EventName)
{
	mrEvent = zEventRef::CreateGPU(_EventName);
	mrEvent.Start();
}

zScopedEventGpu::~zScopedEventGpu()
{
	mrEvent.Stop();
}

}}