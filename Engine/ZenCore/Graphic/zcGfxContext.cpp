#include "zcCore.h"

namespace zcGfx
{

zMap<zU16>::Key32 DrawContext::sdDrawcallCount[2]; 

DrawContext::DrawContext(const zStringHash32& _zContextName, DrawContext* _pParent, const zcRes::GfxRenderPassRef& _rRenderpass)
: mzName(_zContextName)
, mbRootContext( _pParent == nullptr )
, mrRenderpass(_rRenderpass)
{		
	if( _pParent )
	{
		_pParent->mlstChilds.PushTail(*this);
		ReferenceAdd(); //! @todo Clean Have list manage ref count		
	}

	//! @todo Perf Record previous drawcall count
	zU16 uPreviousCount(0);
//	sdDrawcallCount[ (zcMgr::GfxRender.GetFrameCount() + 1) % 2 ].Get(mzName.mhName, uPreviousCount);

	uPreviousCount	= zenMath::Max<zU16>(128, uPreviousCount);
	mbSubmitted		= false;
	marDrawcalls.Reserve( (uPreviousCount*4)/3 ); // Reserve a bit more than previous frame, to reduce array resizing
}

DrawContext::~DrawContext()
{
	//! @todo Clean Have list manage ref count
	DrawContext* pChild = mlstChilds.GetHead();	
	while( pChild )
	{
		DrawContext* pDelChild	= pChild;
		pChild					= mlstChilds.GetNext(*pChild);
		pDelChild->ReferenceRem();
	}
}

void DrawContext::Clear()
{	
	DrawContext* pChildContext = mlstChilds.PopHead();
	while( pChildContext )
	{
		pChildContext->Clear();
		pChildContext = mlstChilds.PopHead();	
	}	
	marDrawcalls.Clear();
}

void DrawContext::Submit()
{
	zenAssertMsg( mbRootContext, "Can only submit a root context");
	SubmitInternal();	
}

void DrawContext::SubmitInternal()
{
	zenAssertMsg( mbSubmitted == false, "Can only submit a context once");
	if( !marDrawcalls.IsEmpty() || !mlstChilds.IsEmpty() )
	{
		zenPerf::zScopedEventGpu Event(mzName);
		mbSubmitted = true;
		if( !marDrawcalls.IsEmpty() )
		{
			//marDrawcalls.Sort(); //! @todo 1 urgent re-add sorting
			zcMgr::GfxRender.Render(marDrawcalls);
		}

		DrawContext* pChildCur = mlstChilds.GetHead();
		while( pChildCur )
		{
			pChildCur->SubmitInternal();
			pChildCur = mlstChilds.GetNext(*pChildCur);
		}	
	}
}

}
