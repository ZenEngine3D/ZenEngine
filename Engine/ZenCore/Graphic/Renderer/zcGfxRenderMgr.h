#pragma once
#ifndef __zCore_Gfx_Renderer_Manager_h__
#define __zCore_Gfx_Renderer_Manager_h__

//#include <Engine/libZenCore/Graphic/EngineGraphicBase.h>

namespace zcGfx
{
//=================================================================================================
//! @brief		zbType::Manager used to control hardware renderer
//! @details	This base class is only used for shared code amongst various 
//!				renderer libraries (directx11, opengl, etc...)\n
//!				For actual libraries specific code, look in the proper 
//!				implementation file
//=================================================================================================	
class ManagerRender_Base : public zbType::Manager
{
ZENClassDeclare(ManagerRender_Base, zbType::Manager)
public:	
								ManagerRender_Base();

	virtual void				FrameBegin( zcRes::GfxWindowRef _FrameWindow );
	virtual void				FrameEnd();
	
	zU64						GetFrameCount();
	ZENInline zcRes::GfxWindowRef	GetWindowCurrent(){return mrWindowCurrent;}

	//! @todo Clean: move to more generic AWformat testing functions file?
	bool						IsDepth( zenConst::eTextureFormat _eTexFormat ) const { return _eTexFormat>=zenConst::keTexFormat__DepthFirst && _eTexFormat<=zenConst::keTexFormat__DepthLast; }
	
protected:
	enum eConstants{kuFramesSaved=10};
	zU64						muFrameCount;	
	zcRes::GfxWindowRef			mrWindowCurrent;
};

}

#include ZENHeaderRenderer( zcGfxRenderMgr )

namespace EMgr { extern zcGfx::ManagerRender GfxRender; }

#endif
