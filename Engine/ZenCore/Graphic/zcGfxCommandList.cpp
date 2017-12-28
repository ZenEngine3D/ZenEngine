#include "zcCore.h"

namespace zcGfx
{

zMap<zU16>::Key32 CommandList_Base::sdDrawcallCount[2]; 

CommandListRef CommandList::Create(const zStringHash32& _zScopeName, const CommandListRef& _rParent, const zcRes::GfxRenderPassRef& _rRenderState)
{
	static zenMem::zAllocatorPool sPoolContext("zcGfx::DrawlistContext", sizeof(CommandList), 512, 128);	
	CommandListRef rContext = zenNew(&sPoolContext) CommandList(_zScopeName, _rParent, _rRenderState);
	return rContext;
}

CommandList::CommandList(const zStringHash32& _zContextName, const CommandListRef& _rParent, const zcRes::GfxRenderPassRef& _rRenderpass)
: Super(_zContextName, _rParent, _rRenderpass)
{
}

CommandList_Base::CommandList_Base(const zStringHash32& _zContextName, const CommandListRef& _rParent, const zcRes::GfxRenderPassRef& _rRenderpass)
: mzName(_zContextName)
, mbInsertBeforeChilds(true)
, mbRootContext(!_rParent.IsValid())
, mrRenderpass(_rRenderpass)
{		
	if( _rParent.IsValid() )
	{
		_rParent->mlstChilds.PushTail(*this);
		ReferenceAdd(); //! @todo Clean Have list manage ref count		
	}

	//! @todo 1 Perf Record previous drawcall count
	zU16 uPreviousCount(0);
//	sdDrawcallCount[ (zcMgr::GfxRender.GetFrameCount() + 1) % 2 ].Get(mzName.mhName, uPreviousCount);

	uPreviousCount	= zenMath::Max<zU16>(128, uPreviousCount);
	mbSubmitted		= false;
	marDrawcalls[0].Reserve( (uPreviousCount*4)/3 ); // Reserve a bit more than previous frame, to reduce array resizing
}

CommandList_Base::~CommandList_Base()
{
	//! @todo Clean Have list manage ref count
	CommandList_Base* pChild = mlstChilds.GetHead();	
	while( pChild )
	{
		CommandList_Base* pDelChild	= pChild;
		pChild							= mlstChilds.GetNext(*pChild);
		pDelChild->ReferenceRem();
	}
}

void CommandList_Base::Clear()
{	
	CommandList_Base* pChildContext = mlstChilds.PopHead();
	while( pChildContext )
	{
		pChildContext->Clear();
		pChildContext = mlstChilds.PopHead();	
	}	
	marDrawcalls[0].Clear();
	marDrawcalls[1].Clear();
}


void CommandList::Submit()
{
	zenAssertMsg( mbRootContext, "Can only submit a root context");
	SubmitInternal();
}

void CommandList::SubmitInternal()
{
	zenAssertMsg( mbSubmitted == false, "Can only submit a context once");
	zcPerf::EventBaseRef rEvent = zcPerf::EventGPU::Create(mzName);	
	rEvent->GPUStart(this);	//Add Scoped Event start at beginning 
	rEvent->GPUStop(this);	//Add Scoped Event stop at end
	marDrawcalls[0].Sort();
	marDrawcalls[1].Sort();
	
	// Submit commands meant to be before children
	if( marDrawcalls[0].IsEmpty() == false )
		zcMgr::GfxRender.SubmitToGPU(this, marDrawcalls[0]);

	// Ask children to Submit their commands
	CommandList* pChildCur = static_cast<CommandList*>(mlstChilds.GetHead());
	while( pChildCur )
	{
		pChildCur->SubmitInternal();
		pChildCur = static_cast<CommandList*>(mlstChilds.GetNext(*pChildCur));
	}

	// Submit commands meant to be after children
	if( marDrawcalls[1].IsEmpty() == false )
		zcMgr::GfxRender.SubmitToGPU(this, marDrawcalls[1]);
}

}
