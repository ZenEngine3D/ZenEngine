#include "zcCore.h"

namespace zcRes
{
	void GfxWindow::SetBackbuffer(zU8 _uBackbufferIndex, const GfxRenderTargetRef& _rBackbufferColor) 
	{ 
		ZENAssert(_uBackbufferIndex < ZENArrayCount(mrBackbufferColor));
		mrBackbufferColor[_uBackbufferIndex] = _rBackbufferColor;
	}

	const GfxRenderTargetRef&	GfxWindow::GetBackbuffer() 
	{		
		return mrBackbufferColor[ zcMgr::GfxRender.GetFrameCount() % ZENArrayCount(mrBackbufferColor) ];
	}
}