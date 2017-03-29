#pragma once

namespace zcGfx
{

class ScopedDrawlist_Base : public zRefCounted
{
zenClassDeclare(ScopedDrawlist_Base, zRefCounted)
zListLink mlnkChild;
typedef zList<ScopedDrawlist_Base, &ScopedDrawlist_Base::mlnkChild, false> TypeListChild;		

public:	
	zenInline const zcRes::GfxRenderPassRef&	GetRenderpass();
	zenInline void								AddCommand( const zEngineRef<zcGfx::Command>& _rCommand );
	void										Clear();
	const zArrayDynamic<zEngineRef<zcGfx::Command>>& GetCommands()const {return marDrawcalls; }
protected:
												ScopedDrawlist_Base(const zStringHash32& _zContextName, ScopedDrawlist* _pParent, const zcRes::GfxRenderPassRef& _rRenderpass);
	virtual										~ScopedDrawlist_Base();
	zStringHash32								mzName;					//!< Name of the context (used for draw events)
	bool										mbSubmitted;
	bool										mbRootContext;			//!< True if there's no parent for this context
	TypeListChild								mlstChilds;				//!< List of childs DrawingContext	
	zcRes::GfxRenderPassRef						mrRenderpass;			//!< Associated renderpass to draw this context
	zArrayDynamic<zEngineRef<zcGfx::Command>>	marDrawcalls;			//!< List of drawcalls for this DrawingContext
	static zMap<zU16>::Key32					sdDrawcallCount[2];		//!< Drawcall count of previous/current frame, of all DrawingContext	
};

}

#include zenHeaderRenderer(zcGfxScopedDrawlist)
#include "zcGfxScopedDrawlist.inl"

namespace zcGfx
{

class ScopedDrawlist : public ScopedDrawlist_HAL
{
zenClassDeclare(ScopedDrawlist, ScopedDrawlist_HAL)
public:	
	void										Submit();
	
protected:
												ScopedDrawlist(const zStringHash32& _zContextName, ScopedDrawlist* _pParent, const zcRes::GfxRenderPassRef& _rRenderpass);
	void										SubmitInternal();
	friend class zenGfx::zScopedDrawlist;
};

}

