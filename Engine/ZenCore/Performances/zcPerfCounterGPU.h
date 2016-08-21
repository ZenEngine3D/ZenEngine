#pragma once
#ifndef __LibZenEngine_Perf_CounterGPU_h__
#define __LibZenEngine_Perf_CounterGPU_h__

//! @todo clean redefine all platform classes to use Class_Base, Class_Platform, Class hierarchy
//! @todo clean settle on _Platform extension name typedef

namespace zcPerf 
{
	//! @brief Basic class for GPU event, code share by all platform
	class EventGPU_Base : public EventBase
	{
	ZENClassDeclare(EventGPU_Base, EventBase)
	public:									
		virtual void	Start();
		virtual void	Stop();

	protected:
						EventGPU_Base(const zStringHash32& _EventName);
	};
}

#include ZENHeaderRenderer(zcPerfCounterGPU)

namespace zcPerf 
{
	//! @brief GPU events. Used to get timing infos from the GPU
	class EventGPU : public EventGPU_Platform
	{
	ZENClassDeclare(EventGPU, EventGPU_Platform)
	public:									
		static zEngineRef<EventBase>	Create(const zStringHash32& _EventName);
	
	protected:
										EventGPU(const zStringHash32& _EventName);
	};

	//! @brief Used to retrieve number of drawcall send to GPU, per type
	class EventGPUCounter : public EventGPU_Base
	{
	ZENClassDeclare(EventGPUCounter, EventGPU_Base)
	public:
		enum eType{ keType_DrawIndexed, keType_Compute, keType_ClearColor, keType_ClearDepth, keType_UpdateIndex, keType_UpdateVertex, keType__Count, keType__Invalid=keType__Count};
		static zEngineRef<EventBase>	Create(eType _eCounterType);
		virtual void					Start();

	protected:
										EventGPUCounter(eType _eCounterType);
		eType							meCounterType;
	};
}

#endif
