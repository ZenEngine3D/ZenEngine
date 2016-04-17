#include "zcCore.h"

namespace zcGfx
{

zcRes::GfxWindowRef gWindowRender;

ManagerRender_Base::ManagerRender_Base()
{
}

void ManagerRender_Base::FrameBegin( zcRes::GfxWindowRef _FrameWindow )
{
	ZENAssertMsg( !gWindowRender.IsValid(), "End previous frame before starting a new one" );
	gWindowRender	= _FrameWindow;
}

void ManagerRender_Base::FrameEnd()
{
	gWindowRender = nullptr;
	++muFrameRendered;
}

}
