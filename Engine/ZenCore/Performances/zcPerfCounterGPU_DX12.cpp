#include "zcCore.h"

//SF DX12 

namespace zcPerf
{
	EventGPU_DX12::EventGPU_DX12(const zStringHash32& _EventName) 
	: Super(_EventName) 
	{
	}

	void EventGPU_DX12::Start()
	{
		Super::Start();
		zcMgr::GfxRender.NamedEventBegin(mzEventName);
		mrQueryTimestampStart = zcGfx::DX12QueryTimestamp::Create();
	}

	void EventGPU_DX12::Stop()
	{
		mrQueryTimestampStop = zcGfx::DX12QueryTimestamp::Create();
		zcMgr::GfxRender.NamedEventEnd();
		Super::Stop();		
	}

	double EventGPU_DX12::GetElapsedMs()
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