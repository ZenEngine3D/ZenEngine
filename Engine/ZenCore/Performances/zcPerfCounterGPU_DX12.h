#pragma once

namespace zcPerf 
{

class EventGPU_DX12 : public EventGPU_Base
{
zenClassDeclare(EventGPU_DX12, EventGPU_Base)
public:
	
	virtual void	GPUStart(const zcGfx::CommandListRef& _rDrawlist);
	virtual void	GPUStop(const zcGfx::CommandListRef& _rDrawlist);
	virtual double	GetElapsedMs();

protected:
					EventGPU_DX12(const zStringHash32& _EventName);
	zUInt			muQueryTimestampStart;
	zUInt			muQueryTimestampStop;
};

}
