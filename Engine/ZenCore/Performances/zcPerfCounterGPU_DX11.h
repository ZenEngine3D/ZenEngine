#pragma once

namespace zcGfx { class QueryTimestamp_DX11; }

namespace zcPerf 
{

class EventGPU_DX11 : public EventGPU_Base
{
zenClassDeclare(EventGPU_DX11, EventGPU_Base)
public:									
	virtual void							GPUStart(const zcGfx::CommandListRef& _rDrawlist);
	virtual void							GPUStop(const zcGfx::CommandListRef& _rDrawlist);
	virtual double							GetElapsedMs();

protected:
											EventGPU_DX11(const zStringHash32& _EventName);
	zEngineRef<zcGfx::QueryTimestamp_DX11>	mrQueryTimestampStart;
	zEngineRef<zcGfx::QueryTimestamp_DX11>	mrQueryTimestampStop;
};

}
