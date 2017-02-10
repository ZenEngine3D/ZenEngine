#pragma once

namespace zcPerf 
{

class EventGPU_DX11 : public EventGPU_Base
{
zenClassDeclare(EventGPU_DX11, EventGPU_Base)
public:									
	virtual void								Start();
	virtual void								Stop();
	virtual double								GetElapsedMs();

protected:
											EventGPU_DX11(const zStringHash32& _EventName);
	zEngineRef<zcGfx::QueryTimestamp_DX11>	mrQueryTimestampStart;
	zEngineRef<zcGfx::QueryTimestamp_DX11>	mrQueryTimestampStop;
};

}
