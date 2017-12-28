#include "zcCore.h"

namespace zen {	namespace zenPerf
{

zEventRef zEventRef::CreateCPU(const zStringHash32& _EventName)
{
	return zcPerf::EventCPU::Create(_EventName).Get();
}

void zEventRef::Start()
{
	GetSafe()->CPUStart();
}

void zEventRef::Stop()
{
	GetSafe()->CPUStop();
}

zEventRef zEventRef::GetFirstChild()const
{
	return GetSafe()->GetFirstChild().Get();
}

zEventRef zEventRef::GetNext() const
{
	return zcPerf::EventBase::TypeListChild::GetNext(*GetSafe());
}

zEventRef zEventRef::GetPrev() const
{
	return zcPerf::EventBase::TypeListChild::GetPrev(*GetSafe());
}

const zStringHash32& zEventRef::GetName() const
{
	return GetSafe()->GetName();
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

}}