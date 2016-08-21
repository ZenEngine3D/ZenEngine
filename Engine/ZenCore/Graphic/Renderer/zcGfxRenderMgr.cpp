#include "zcCore.h"

namespace zcGfx
{

zcRes::GfxWindowRef grWindowRender;

ManagerRender_Base::ManagerRender_Base()
{
}

void ManagerRender_Base::FrameBegin( zcRes::GfxWindowRef _FrameWindow )
{
	zenAssertMsg( !grWindowRender.IsValid(), "End previous frame before starting a new one" );
	grWindowRender	= _FrameWindow;
}

void ManagerRender_Base::FrameEnd()
{
	grWindowRender = nullptr;
	++muFrameRendered;
}

}
