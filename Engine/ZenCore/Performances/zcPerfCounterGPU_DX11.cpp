#include "zcCore.h"



namespace zcPerf
{
	void zeEventGPU::Start()
	{
		zcMgr::GfxRender.NamedEventBegin(mzEventName);
	}

	void zeEventGPU::Stop()
	{
		zcMgr::GfxRender.NamedEventEnd();
	}
}