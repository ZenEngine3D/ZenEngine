#include "zcCore.h"

namespace zcMgr 
{ 
	zcGfx::ManagerRender_HAL GfxRender; 	
}

namespace zcGfx
{

zcRes::GfxWindowRef grWindowRender;

ManagerRender_Base::ManagerRender_Base()
{
	mrFrameContext	= zenGfx::zScopedDrawlist::Create("PreFrame");
}

void ManagerRender_Base::FrameBegin( zcRes::GfxWindowRef _FrameWindow )
{
	zenAssertMsg( !grWindowRender.IsValid(), "End previous frame before starting a new one" );
	grWindowRender		= _FrameWindow;
	
	if( mrFrameContext.IsValid() )
		mrFrameContext.Submit();

	zcGfx::Command::ResetCommandCount();
	mrFrameContext	= zenGfx::zScopedDrawlist::Create("PostFrame");
}

void ManagerRender_Base::FrameEnd()
{
	if( mrFrameContext.IsValid() )
		mrFrameContext.Submit();
	mrFrameContext	= zenGfx::zScopedDrawlist::Create("PreFrame");

	grWindowRender = nullptr;
	++muFrameRendered;
}

}
