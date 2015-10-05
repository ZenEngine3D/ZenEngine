#pragma once
#ifndef __zCore_Gfx_Renderer_Drawcall_h__
#define __zCore_Gfx_Renderer_Drawcall_h__

namespace zcGfx
{
//=================================================================================================
//! @brief		
//! @details	
//!				
//=================================================================================================	
//! @todo Urgent current work
class Drawcall : public zRefCounted
{
ZENClassDeclare(Drawcall, zRefCounted)
public:											
	static zEngineRef<Drawcall> Create(	const zcRes::GfxRenderPassRef& _rRenderPass, const zcRes::GfxMeshStripRef& _rMeshStrip,	const float& _fPriority );

//protected:	
	union RenderStateSortID
	{
		// Put highest priority state change item last
		struct
		{
		float	mfPriority;		
		zU32	muShaderBindingID	: 21; 
		zU32	muShaderMode		: 3;
		zU32	muRenderPassID		: 8;
		};

		zU64	mSortKey;
	};
	RenderStateSortID					mSortId;	
	zcRes::GfxRenderPassProxyRef		mrRenderPass;
	zcRes::GfxMeshStripProxyRef			mrMeshStrip;

private:
	Drawcall(){};
};

}

#endif
