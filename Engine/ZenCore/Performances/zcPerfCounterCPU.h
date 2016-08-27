#pragma once
#ifndef __LibZenEngine_Perf_CounterCPU_h__
#define __LibZenEngine_Perf_CounterCPU_h__

namespace zcPerf 
{
	//! @brief Basic class for CPU event, code share by all platform
	class EventCPU_Base : public EventBase
	{
	zenClassDeclare(EventCPU_Base, EventBase)
	public:									
		virtual void	Start();
		virtual void	Stop();

	protected:
						EventCPU_Base(const zStringHash32& _EventName);
	};
}

#include ZENHeaderPlatform(zcPerfCounterCPU)

namespace zcPerf 
{
	class EventCPU : public EventCPU_Platform
	{
	zenClassDeclare(EventCPU, EventCPU_Platform)
	public:						
		static zEngineRef<EventBase>	Create(const zStringHash32& _EventName);

	protected:
										EventCPU(const zStringHash32& _EventName) : Super(_EventName) {}
	};
}

#endif
