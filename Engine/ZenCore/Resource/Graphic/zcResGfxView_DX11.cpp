#include "zcCore.h"

namespace zcRes
{

GfxViewProxy_DX11::GfxViewProxy_DX11()
{
}

GfxViewProxy_DX11::~GfxViewProxy_DX11()
{
}

bool GfxViewProxy_DX11::Initialize(class GfxView& _Owner)
{
	const GfxView::ResDataRef& rResData = _Owner.GetResData();
	ZENAssert(rResData.IsValid());
	ZENDbgCode(mpOwner = &_Owner);

	marProxTargetColor.SetCount( rResData->maTargetColorID.Count() );
	for(int idx(0), count(marProxTargetColor.Count()); idx<count; ++idx)
		marProxTargetColor[idx] = GetResourceProxy<GfxRenderTargetRef>(rResData->maTargetColorID[idx]);
	mrProxTargetDepth =  GetResourceProxy<GfxRenderTargetRef>(rResData->mTargetDepthID);
	return true;
}

void GfxViewProxy_DX11::Clear( bool _bClearColor, const zVec4F& _vRGBA, bool _bClearDepth, float _fDepth, bool _bClearStencil, zU8 _uStencil )
{
	if( _bClearColor )
	{
		for(zUInt rtIdx(0), rtCount(marProxTargetColor.Count()); rtIdx<rtCount; ++rtIdx)
		{
			if( marProxTargetColor[rtIdx].IsValid() )
				marProxTargetColor[rtIdx]->Clear(_vRGBA);
		}
	}
	if( mrProxTargetDepth.IsValid()&& (_bClearDepth || _bClearStencil) )
	{
		mrProxTargetDepth->Clear(_fDepth, _uStencil, _bClearDepth, _bClearStencil);
	}
}

}