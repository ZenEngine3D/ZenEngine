#include "zcCore.h"

namespace zcPerf
{
	EventGPU_DX11::EventGPU_DX11(const zStringHash32& _EventName) 
	: Super(_EventName) 
	{
	}

	void EventGPU_DX11::GPUStart(const zcGfx::CommandListRef& _rDrawlist)
	{		
		zcGfx::CommandList::ScopedInsertPoint InsertPoint(*_rDrawlist.Get(), true);
		Super::GPUStart(_rDrawlist);
		mrQueryTimestampStart = zcGfx::QueryTimestamp_DX11::Create();
		zcGfx::CommandGPUScopedEvent::Add(_rDrawlist, mzEventName.mzName, zcGfx::CommandGPUScopedEvent::keEventStart);		
		zcGfx::CommandQueryEnd_DX11::Add(_rDrawlist, mrQueryTimestampStart->GetQuery(), true);
	}

	void EventGPU_DX11::GPUStop(const zcGfx::CommandListRef& _rDrawlist)
	{
		zcGfx::CommandList::ScopedInsertPoint InsertPoint(*_rDrawlist.Get(), false);
		mrQueryTimestampStop = zcGfx::QueryTimestamp_DX11::Create();
		zcGfx::CommandGPUScopedEvent::Add(_rDrawlist, mzEventName.mzName, zcGfx::CommandGPUScopedEvent::keEventEnd);
		zcGfx::CommandQueryEnd_DX11::Add(_rDrawlist, mrQueryTimestampStop->GetQuery(), false);
		Super::GPUStop(_rDrawlist);		
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