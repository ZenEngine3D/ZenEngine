#include "zcCore.h"

namespace zcGfx
{

#if 0
class  : public zRefCounted
{
ZENClassDeclare(DrawingContext, zRefCounted)
zListLink mlnkChild;
typedef zList<DrawingContext, &DrawingContext::mlnkChild> TypeListChild;
public:
		
protected:
										
	zStringHash							mzName;
	TypeListChild						mlstChilds;
	zcRes::GfxRenderPassRef				mrRenderpass;
	zArrayDynamic<zenRes::zGfxDrawcall>	marDrawcalls;
};
#endif

zMap<zU16>::Key32 DrawContext::sdDrawcallCount[2]; 

DrawContext::DrawContext(const zStringHash32& _zContextName, DrawContext* _pParent, const zcRes::GfxRenderPassRef& _rRenderpass)
: mzName(_zContextName)
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
	uPreviousCount = zenMath::Max<zU16>(128, uPreviousCount);
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

void DrawContext::Submit()
{
	if( !marDrawcalls.IsEmpty() || !mlstChilds.IsEmpty() )
	{
		zenPerf::zScopedEventGpu EmitEvent(mzName);
		zcMgr::GfxRender.Render(marDrawcalls);

		DrawContext* pChildCur = mlstChilds.GetHead();
		while( pChildCur )
		{
			pChildCur->Submit();
			pChildCur = mlstChilds.GetNext(*pChildCur);
		}
	}
	
}

}
