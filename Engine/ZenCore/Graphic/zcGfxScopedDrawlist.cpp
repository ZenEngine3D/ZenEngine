#include "zcCore.h"

namespace zcGfx
{

zMap<zU16>::Key32 ScopedDrawlist_Base::sdDrawcallCount[2]; 

ScopedDrawlist::ScopedDrawlist(const zStringHash32& _zContextName, ScopedDrawlist* _pParent, const zcRes::GfxRenderPassRef& _rRenderpass)
: Super(_zContextName, _pParent, _rRenderpass)
{
}

ScopedDrawlist_Base::ScopedDrawlist_Base(const zStringHash32& _zContextName, ScopedDrawlist* _pParent, const zcRes::GfxRenderPassRef& _rRenderpass)
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

ScopedDrawlist_Base::~ScopedDrawlist_Base()
{
	//! @todo Clean Have list manage ref count
	ScopedDrawlist_Base* pChild = mlstChilds.GetHead();	
	while( pChild )
	{
		ScopedDrawlist_Base* pDelChild	= pChild;
		pChild							= mlstChilds.GetNext(*pChild);
		pDelChild->ReferenceRem();
	}
}

void ScopedDrawlist_Base::Clear()
{	
	ScopedDrawlist_Base* pChildContext = mlstChilds.PopHead();
	while( pChildContext )
	{
		pChildContext->Clear();
		pChildContext = mlstChilds.PopHead();	
	}	
	marDrawcalls.Clear();
}

void ScopedDrawlist::Submit()
{
	zenAssertMsg( mbRootContext, "Can only submit a root context");
	SubmitInternal();	
}

void ScopedDrawlist::SubmitInternal()
{
	zenAssertMsg( mbSubmitted == false, "Can only submit a context once");
	if( !marDrawcalls.IsEmpty() || !mlstChilds.IsEmpty() )
	{
		zenPerf::zScopedEventGpu Event(mzName);
		mbSubmitted = true;
		if( !marDrawcalls.IsEmpty() )
		{
			marDrawcalls.Sort();
			zcMgr::GfxRender.Render(*this);
		}

		ScopedDrawlist* pChildCur = static_cast<ScopedDrawlist*>(mlstChilds.GetHead());
		while( pChildCur )
		{
			pChildCur->SubmitInternal();
			pChildCur = static_cast<ScopedDrawlist*>(mlstChilds.GetNext(*pChildCur));
		}	
	}
}

}
