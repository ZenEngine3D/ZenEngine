#include "zcCore.h"

namespace zcPerf
{
	EventGPU_DX11::EventGPU_DX11(const zStringHash32& _EventName) 
	: Super(_EventName) 
	{
	}

	void EventGPU_DX11::Start(  )
	{
		Super::Start();
		zcMgr::GfxRender.NamedEventBegin(mzEventName);
		mrQueryTimestampStart = zcGfx::QueryTimestamp_DX11::Create();
	}

	void EventGPU_DX11::Stop(  )
	{
		mrQueryTimestampStop = zcGfx::QueryTimestamp_DX11::Create();
		zcMgr::GfxRender.NamedEventEnd();
		Super::Stop();		
	}

	double EventGPU_DX11::GetElapsedMs()
	{
		if( muTimeElapsed == 0 && mrQueryTimestampStart.IsValid() && mrQueryTimestampStop.IsValid() && mrQueryTimestampStart->GetTimestampUSec() > 0 && mrQueryTimestampStop->GetTimestampUSec() > 0)
		{
			zU64 uTimeStart	= mrQueryTimestampStart->GetTimestampUSec();
			zU64 uTimeStop	= mrQueryTimestampStop->GetTimestampUSec();
			muTimeStart		= uTimeStart;
			muTimeElapsed	= uTimeStop - uTimeStart;
		}

		return Super::GetElapsedMs();
	}
}