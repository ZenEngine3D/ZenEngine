#pragma once

namespace zcGfx
{
extern zcRes::GfxWindowRef	grWindowRender;	//!< Window being currently rendered //! @todo clean rethink global naming convention. g in front of everything?

//=================================================================================================
//! @brief		zbType::Manager used to control hardware renderer
//! @details	This base class is only used for shared code amongst various 
//!				renderer libraries (directx11, opengl, etc...)\n
//!				For actual libraries specific code, look in the proper 
//!				implementation file
//=================================================================================================	
class ManagerRender_Base : public zbType::Manager
{
zenClassDeclare(ManagerRender_Base, zbType::Manager)
public:	
											ManagerRender_Base();

	virtual void							FrameBegin( zcRes::GfxWindowRef _FrameWindow );
	virtual void							FrameEnd();	
	const zenGfx::zScopedDrawlist&			GetFrameContext(){ return mrFrameContext; };

	//! @todo Clean: move to more generic ZENFormat testing functions file?
	bool									IsDepth( zenConst::eTextureFormat _eTexFormat ) const { return _eTexFormat>=zenConst::keTexFormat__DepthFirst && _eTexFormat<=zenConst::keTexFormat__DepthLast; }
	zenInline zUInt							GetFrameRendered()const;
protected:
	zUInt									muFrameRendered = 0;
	zenGfx::zScopedDrawlist				mrFrameContext;		//!<< Used to push command without having a particular context. Executed both at start/end of current frame
};

zUInt ManagerRender_Base::GetFrameRendered()const
{
	return muFrameRendered;
}

}

#include zenHeaderRenderer( zcGfxRenderMgr )

namespace zcMgr { 
	extern zcGfx::ManagerRender_HAL GfxRender; 	
}
