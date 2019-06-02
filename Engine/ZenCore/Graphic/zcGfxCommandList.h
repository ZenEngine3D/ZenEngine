#pragma once

namespace zcGfx
{
using CommandRef = zEngineRef<zcGfx::Command>;

class CommandList_Base : public zRefCounted, public zListItem<1,1>
{
zenClassDeclare(CommandList_Base, zRefCounted)
using TypeListChild	= zListRef<CommandList_Base, 0, zListItem<1,1> >;
public:	
	struct ScopedInsertPoint
	{
		ScopedInsertPoint(CommandList_Base& _CommandList, bool _bInsertBefore=true)
		: mCommandList(_CommandList)
		, mbOriginalValue(_CommandList.IsBeforeChildren())
		{
			mCommandList.SetBeforeChildren(_bInsertBefore);		
		}
		~ScopedInsertPoint()
		{
			mCommandList.SetBeforeChildren(mbOriginalValue);	
		}
		CommandList_Base&	mCommandList;
		bool				mbOriginalValue;
	};

	zenInline const zcRes::GfxRenderPassRef&	GetRenderpass();
	zenInline void								AddCommand( const zEngineRef<zcGfx::Command>& _rCommand );
	zenInline void								SetBeforeChildren(bool _bBeforeChild=true);
	zenInline bool								IsBeforeChildren()const;
	zenInline bool								IsEmpty()const;
	void										Clear();
	const zArrayDynamic<CommandRef>&			GetCommands(bool _bBeforeChild=true)const { return marDrawcalls[_bBeforeChild]; }
protected:
												CommandList_Base(const zStringHash32& _zScopeName, const CommandListRef& _rParent, const zcRes::GfxRenderPassRef& _rRenderpass);
	virtual										~CommandList_Base();
	zStringHash32								mzName;					//!< Name of the scoped drawlist (used for draw events)
	bool										mbInsertBeforeChilds;	//!< If we should add new commands before or after this context children
	bool										mbSubmitted;
	bool										mbRootContext;			//!< True if there's no parent for this context
	TypeListChild								mlstChilds;				//!< List of childs DrawingContext	
	zcRes::GfxRenderPassRef						mrRenderpass;			//!< Associated renderpass to draw this context
	zArrayDynamic<CommandRef>					marDrawcalls[2];		//!< List of command to will be run for this Context ([0]-Before Children, [1]-After children)
	static zMap<zU16>::Key32					sdDrawcallCount[2];		//!< Command count of previous/current frame, by Context NameHash @todo 1 re-impement this
	friend ManagerRender_HAL;
};

}

#include zenHeaderRenderer(zcGfxCommandList)
#include "zcGfxCommandList.inl"

namespace zcGfx
{

using CommandListRef = zEngineRef<class CommandList>;

class CommandList : public CommandList_HAL
{
zenClassDeclare(CommandList, CommandList_HAL)
public:	
	static CommandListRef						Create(const zStringHash32& _zScopeName, const CommandListRef& _rParent, const zcRes::GfxRenderPassRef& _rRenderState);
	void										Submit();
	
protected:
	void										SubmitInternal();
												CommandList(const zStringHash32& _zContextName, const CommandListRef& _rParent, const zcRes::GfxRenderPassRef& _rRenderpass);
};


}

