#include "zcCore.h"

namespace zcGfx
{

zMap<zU16>::Key32 CommandList_Base::sdDrawcallCount[2]; 

CommandListRef CommandList::Create(const zStringHash32& _zScopeName, const CommandListRef& _rParent, const zcRes::GfxRenderPassRef& _rRenderState)
{
	CommandListRef rContext = zenMem::NewPool<CommandList>(_zScopeName, _rParent, _rRenderState);
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
		_rParent->mlstChilds.push_back(*this);

	//! @todo 1 Perf Record previous drawcall count
	zUInt uPreviousBeforeCount(0);
	zUInt uPreviousAfterCount(0);
//	sdDrawcallCount[ (zcMgr::GfxRender.GetFramesize() + 1) % 2 ].Get(mzName.mhName, uPreviousCount);

	mbSubmitted				= false;
	uPreviousBeforeCount	= zenMath::Max<zUInt>(64, uPreviousBeforeCount);
	uPreviousAfterCount		= zenMath::Max<zUInt>(64, uPreviousAfterCount);	
//	marDrawcalls[0].Reserve( (uPreviousBeforeCount*4)/3 ); // Reserve a bit more than previous frame, to reduce array resizing
//	marDrawcalls[1].Reserve( (uPreviousAfterCount*4)/3 ); // Reserve a bit more than previous frame, to reduce array resizing
}

CommandList_Base::~CommandList_Base()
{
	Clear();
}

void CommandList_Base::Clear()
{	
	CommandList_Base* pChildContext = mlstChilds.pop_front();
	while( pChildContext )
	{
		pChildContext->Clear();
		pChildContext = mlstChilds.pop_front();	
	}	

	marDrawcalls[0].clear();
	marDrawcalls[1].clear();
}


void CommandList::Submit()
{
	zenAssertMsg( mbRootContext, "Can only submit a root context");
	SubmitInternal();
}

void CommandList::SubmitInternal()
{
	zenAssertMsg( mbSubmitted == false, "Can only submit a context once");

	if( !mbRootContext )//! @todo 1 Look into why this is causing 'End without GetData' warning on DX11
	{
		zcPerf::EventBaseRef rEvent = zcPerf::EventGPU::Create(mzName);	
		rEvent->GPUStart(this);	//Add Scoped Event start at beginning 
		rEvent->GPUStop(this);	//Add Scoped Event stop at end
	}
	marDrawcalls[0].Sort();
	marDrawcalls[1].Sort();
	
	// Submit commands meant to be before children
	if( marDrawcalls[0].empty() == false )
		zcMgr::GfxRender.SubmitToGPU(this, marDrawcalls[0]);

	// Ask children to Submit their commands
	for(auto& Item : mlstChilds)
		static_cast<CommandList*>(&Item)->SubmitInternal();

	// Submit commands meant to be after children
	if( marDrawcalls[1].empty() == false )
		zcMgr::GfxRender.SubmitToGPU(this, marDrawcalls[1]);
}

}
