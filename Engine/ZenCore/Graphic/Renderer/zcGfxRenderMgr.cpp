#include "zcCore.h"

namespace zcGfx
{
ManagerRender_Base::ManagerRender_Base()
{
	mafFrameElapsedMs.SetCount(32);
	zenMem::Zero(mafFrameElapsedMs.First(), mafFrameElapsedMs.SizeMem());
	muFramePreviousTime = zenSys::GetTimeUSec();
}

void ManagerRender_Base::FrameBegin( zcRes::GfxWindowRef _FrameWindow )
{
	ZENAssertMsg( !mrWindowCurrent.IsValid(), "End previous frame before starting a new one" );		
	mrWindowCurrent			= _FrameWindow;
	if(mbStatsUpdate )
	{
		zU64 uElapsed			= zenSys::GetTimeUSec() - muFramePreviousTime;
		float fElapsedMs		= float(uElapsed) / 1000.0f;
		muFramePreviousTime		= zenSys::GetTimeUSec();
		mfFrameAverageMs		= (float(mfFrameAverageMs) * 99.0f + fElapsedMs) / 100.0f;
		mafFrameElapsedMs[muStatsFrame%mafFrameElapsedMs.Count()] = fElapsedMs;
		++muStatsFrame;
	}
	++muFrameCount;
}

void ManagerRender_Base::FrameEnd()
{
	mrWindowCurrent = nullptr;
}

zU64 ManagerRender_Base::GetFrameCount()
{
	return muFrameCount;
}


}