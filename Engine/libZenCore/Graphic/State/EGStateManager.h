#pragma once
#ifndef __LibGraphic_State_Manager_h__
#define __LibGraphic_State_Manager_h__ 

#include <Engine/libZenCore/Graphic/EngineGraphicBase.h>

namespace EGfx
{

struct ConfigBlend
{
	enum eBlend{
		keBlend_None, 
		keAlphaMask, 
		keBlend_Alpha, 
		keBlend_Add, 
		keBlend_Sub, 
		keBlend_Modulate,
		keBlend_Min,
		keBlend_Max,
		keBlend__Count
	};
	enum eWriteMask{
		keWrite_None	=0,
		keWrite_R		=0x01, 
		keWrite_G		=0x02, 
		keWrite_B		=0x04, 
		keWrite_A		=0x08, 
		keWrite_RGB		=keWrite_R|keWrite_G|keWrite_B, 
		keWrite_RGBA	=keWrite_R|keWrite_G|keWrite_B|keWrite_A
	};
	eBlend	meBlendRGB;
	eBlend	meBlendAlpha;
	float	mfAlphaRef;
	zUInt	muWriteMask;

	ZENInline bool operator==(const ConfigBlend& _Compare)
	{
		return	meBlendRGB		== _Compare.meBlendRGB		&&
				meBlendAlpha	== _Compare.meBlendAlpha	&&
				mfAlphaRef		== _Compare.mfAlphaRef		&&
				muWriteMask		== _Compare.muWriteMask;
	}
	ZENInline bool operator!=(const ConfigBlend& _Compare)
	{
		return !(*this == _Compare);
	}
};

struct ConfigDepthStencil
{
	enum eCompare{
		keDepth_Always, 
		keDepth_Equal, 
		keDepth_NEqual,
		keDepth_GreaterE, 
		keDepth_Greater, 		
		keDepth_LessE, 
		keDepth_Less,
		keDepth__Count
	};
	enum eStencilOp{
		keStencil_NoChange, 
		keStencil_Set, 
		keStencil_Add, 
		keStencil_Sub,
		keStencil__Count,
	};

	eCompare	meDepthTest;
	eCompare	meStencilTest;
	eStencilOp	meStencilOp;
	zS8			muStencilRef;
	bool		mbDepthWrite;

	ZENInline bool operator==(const ConfigDepthStencil& _Compare)
	{
		return 	meDepthTest		== _Compare.meDepthTest		&&
				meStencilTest	== _Compare.meStencilTest	&&
				meStencilOp		== _Compare.meStencilOp		&&
				muStencilRef	== _Compare.muStencilRef	&&	
				mbDepthWrite	== _Compare.mbDepthWrite;
	}
	ZENInline bool operator!=(const ConfigDepthStencil& _Compare)
	{
		return !(*this==_Compare);
	}
};

class ManagerState_Base : public zbType::Manager
{
ZENClassDeclare(ManagerState_Base, zbType::Manager)
public:
					ManagerState_Base();
	
protected:
};

};

#include ZENHeaderRenderer( EGStateManager )

namespace EMgr { extern EGfx::ManagerState GfxState; }

#endif
