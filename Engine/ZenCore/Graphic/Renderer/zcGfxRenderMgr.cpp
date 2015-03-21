#include "zcCore.h"

namespace zcGfx
{
ManagerRender_Base::ManagerRender_Base()
:	muFrameCount(0)
/*
,	muFramesTimeTotal(0)
,	muFrameTimeIndex(0)
*/
{
	//muTimeLast = zenSys::GetTimeUSec();
	//memset(muFramesTimes, 0, sizeof(muFramesTimes));
}

void ManagerRender_Base::FrameBegin( zcRes::GfxWindowRef _FrameWindow )
{
	ZENAssertMsg( !mrWindowCurrent.IsValid(), "End previous frame before starting a new one" );
	_FrameWindow->PerformResize();
	mrWindowCurrent = _FrameWindow;
	/*
	//! @todo Missing: Stats per window
	zU64 uElapsed					= zenSys::GetTimeUSec() - muTimeLast;
	muTimeLast						= zenSys::GetTimeUSec();
	muFramesTimeTotal				+= uElapsed;
	muFrameTimeIndex				= (muFrameTimeIndex + 1) % kuFramesSaved;
	muFramesTimes[muFrameTimeIndex]	= static_cast<zU32>(uElapsed);
	zU64 uTimeAvg=0;
	for(int i=0; i<kuFramesSaved; ++i)
		uTimeAvg += muFramesTimes[i];
	mfFrameTime						= static_cast<float>(double(uTimeAvg) / (double(kuFramesSaved) * 1000.0));
	*/
	++muFrameCount;
}

void ManagerRender_Base::FrameEnd()
{
	mrWindowCurrent = NULL;
}

zU64	ManagerRender_Base::GetFrameCount()
{
	return muFrameCount;
}


}