#pragma once

//! @todo 2 clean redefine all platform classes to use Class_Base, Class_HAL, Class hierarchy

namespace zcPerf 
{
	//! @brief Basic class for GPU event, code share by all platform
	class EventGPU_Base : public EventBase
	{
	zenClassDeclare(EventGPU_Base, EventBase)
	public:									
		virtual void	GPUStart(const zcGfx::CommandListRef& _rDrawlist);
		virtual void	GPUStop(const zcGfx::CommandListRef& _rDrawlist);
	protected:
						EventGPU_Base(const zStringHash32& _EventName);
	};
}

#include zenHeaderRenderer(zcPerfCounterGPU)

namespace zcPerf 
{
	//! @brief GPU events. Used to get timing infos from the GPU
	class EventGPU : public EventGPU_HAL
	{
	zenClassDeclare(EventGPU, EventGPU_HAL)	
	public:									
		static EventBaseRef		Create(const zStringHash32& _EventName);
		virtual void			GPUStart(const zcGfx::CommandListRef& _rDrawlist);
		virtual void			GPUStop(const zcGfx::CommandListRef& _rDrawlist);
	protected:
								EventGPU(const zStringHash32& _EventName);
	};

	//! @brief Used to retrieve number of drawcall send to GPU, per type
	class EventGPUCounter : public EventCPU_Base
	{
	zenClassDeclare(EventGPUCounter, EventCPU_Base)
	public:
		enum eType{ keType_DrawIndexed, keType_Compute, keType_ClearColor, keType_ClearDepth, keType_UpdateIndex, keType_UpdateVertex, 
					keType_UpdateBuffer, keType_UpdateTexture, keType_Query, keType__Count, keType__Invalid=keType__Count};
		
		static EventBaseRef		Create(eType _eCounterType);
		virtual void			CPUStart();
		
	protected:
								EventGPUCounter(eType _eCounterType);
		eType					meCounterType;
	};
}
