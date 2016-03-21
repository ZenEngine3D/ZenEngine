#pragma once
#ifndef __zCore_Gfx_Renderer_Context_h__
#define __zCore_Gfx_Renderer_Context_h__

namespace zcGfx
{

class DrawContext : public zRefCounted
{
ZENClassDeclare(DrawContext, zRefCounted)
zListLink mlnkChild;
typedef zList<DrawContext, &DrawContext::mlnkChild> TypeListChild;		

public:
	
	ZENInline const zcRes::GfxRenderPassRef&	GetRenderpass();
	ZENInline void								AddCommand( const zEngineRef<zcGfx::Command>& _rCommand );
	void										Submit();
protected:
												DrawContext(const zStringHash32& _zContextName, DrawContext* _pParent, const zcRes::GfxRenderPassRef& _rRenderpass);
												~DrawContext();
	zStringHash32								mzName;					//!< Name of the context (used for drawevents)
	TypeListChild								mlstChilds;				//!< List of childs DrawingContext
	zcRes::GfxRenderPassRef						mrRenderpass;			//!< Associated renderpass to draw this context
	zArrayDynamic<zEngineRef<zcGfx::Command>>	marDrawcalls;			//!< List of drawcalls for this DrawingContext
	static zMap<zU16>::Key32					sdDrawcallCount[2];		//!< Drawcall count of previous/current frame, of all DrawingContext
	friend class zenGfx::zContext;
};

}

#include "zcGfxContext.inl"

#endif
