#include "zcCore.h"

namespace zcGfx
{
ManagerRender_Base::ManagerRender_Base()
{
	mafFrameElapsedMs.SetCount(100);
	zenMem::Zero(mafFrameElapsedMs.First(), mafFrameElapsedMs.SizeMem());
	muFramePreviousTime = zenSys::GetTimeUSec();
}

void ManagerRender_Base::FrameBegin( zcRes::GfxWindowRef _FrameWindow )
{
	ZENAssertMsg( !mrWindowCurrent.IsValid(), "End previous frame before starting a new one" );		
	mrWindowCurrent			= _FrameWindow;
	zU32 uElapsed			= zenSys::GetTimeUSec() - muFramePreviousTime;
	float fElapsedMs		= float(uElapsed) / 1000.0f;
	muFramePreviousTime		= zenSys::GetTimeUSec();
	mfFrameAverageMs		= (float(mfFrameAverageMs) * 99.0f + fElapsedMs) / 100.0f;
	mafFrameElapsedMs[muFrameCount%mafFrameElapsedMs.Count()] = fElapsedMs;

	++muFrameCount;
}

void ManagerRender_Base::FrameEnd()
{
	mrWindowCurrent = NULL;
}

zU64 ManagerRender_Base::GetFrameCount()
{
	return muFrameCount;
}


}