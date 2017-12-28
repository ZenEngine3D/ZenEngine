#include "zcCore.h"

//SF DX12 

namespace zcPerf
{
	EventGPU_DX12::EventGPU_DX12(const zStringHash32& _EventName) 
	: Super(_EventName) 
	{		
	}

	void EventGPU_DX12::GPUStart(const zcGfx::CommandListRef& _rDrawlist)
	{
		zcGfx::CommandList::ScopedInsertPoint InsertPoint(*_rDrawlist.Get(), true);
		Super::GPUStart(_rDrawlist);
		muQueryTimestampStart = zcMgr::GfxRender.GetQueryTimestamp().GetNewQuery();
		zcGfx::CommandQueryEnd_DX12::Add(_rDrawlist, &zcMgr::GfxRender.GetQueryTimestamp(), muQueryTimestampStart, true );
		zcGfx::CommandGPUScopedEvent::Add(_rDrawlist, mzEventName.mzName, zcGfx::CommandGPUScopedEvent::keEventStart);
	}

	void EventGPU_DX12::GPUStop(const zcGfx::CommandListRef& _rDrawlist)
	{
		zcGfx::CommandList::ScopedInsertPoint InsertPoint(*_rDrawlist.Get(), false);
		muQueryTimestampStop = zcMgr::GfxRender.GetQueryTimestamp().GetNewQuery();
		zcGfx::CommandQueryEnd_DX12::Add(_rDrawlist, &zcMgr::GfxRender.GetQueryTimestamp(), muQueryTimestampStop, false );
		zcGfx::CommandGPUScopedEvent::Add(_rDrawlist, mzEventName.mzName, zcGfx::CommandGPUScopedEvent::keEventEnd);
		Super::GPUStop(_rDrawlist);		
	}

	double EventGPU_DX12::GetElapsedMs()
	{
		if( muTimeElapsed == 0 &&  zcMgr::GfxRender.GetQueryTimestamp().IsQueryReady(muQueryTimestampStart) && zcMgr::GfxRender.GetQueryTimestamp().IsQueryReady(muQueryTimestampStop) )
 		{
 			zU64 uTimeStart	= zcMgr::GfxRender.GetQueryTimestamp().QueryResult<zU64>(muQueryTimestampStart);
 			zU64 uTimeStop	= zcMgr::GfxRender.GetQueryTimestamp().QueryResult<zU64>(muQueryTimestampStop);			 			
 			muTimeElapsed	= (uTimeStop - uTimeStart)*1000*1000 / zcMgr::GfxRender.GetQueryTimestampFreq();
			muTimeStart		= uTimeStart*1000*1000 / zcMgr::GfxRender.GetQueryTimestampFreq();
 		}

		return Super::GetElapsedMs();
	}
}