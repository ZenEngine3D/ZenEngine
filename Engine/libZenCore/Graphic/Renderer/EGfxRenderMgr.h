#pragma once
#ifndef __LibGraphic_Renderer_Manager_h__
#define __LibGraphic_Renderer_Manager_h__

#include <Engine/libZenCore/Graphic/EngineGraphicBase.h>

namespace EGfx
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
AWClassDeclare(ManagerRender_Base, zbType::Manager)
public:	
								ManagerRender_Base();

	virtual void				FrameBegin( ERes::GfxWindowRef _FrameWindow );
	virtual void				FrameEnd();
	
	zenU64						GetFrameCount();
	inline ERes::GfxWindowRef	GetWindowCurrent(){return mrWindowCurrent;}

	//! @todo move to more generic AWformat testing functions file?
	bool						IsDepth( awconst::eTextureFormat _eTexFormat ) const { return _eTexFormat>=awconst::keTexFormat__DepthFirst && _eTexFormat<=awconst::keTexFormat__DepthLast; }
	
protected:
	enum eConstants{kuFramesSaved=10};
	zenU64						muFrameCount;	
	ERes::GfxWindowRef			mrWindowCurrent;
};

}

#include AWHeaderRenderer( EGfxRenderMgr )

namespace EMgr { extern EGfx::ManagerRender GfxRender; }

#endif
