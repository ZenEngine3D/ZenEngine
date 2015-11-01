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
	ZENAssert(marProxTargetColor.Count() < D3D11_SIMULTANEOUS_RENDER_TARGET_COUNT);
	ZENDbgCode(mpOwner = &_Owner);
	
	zcRes::GfxRenderTargetRef rRTDepthSurface = rResData->mRTDepthConfig.mrTargetSurface;
	mrProxTargetDepth	= rRTDepthSurface->GetProxy();
	mpDepthView			= mrProxTargetDepth.IsValid() ? mrProxTargetDepth->mpTargetDepthView : NULL;

	muColorCount		= 0;
	zenMem::Zero(mpColorViews, sizeof(mpColorViews) );
	marProxTargetColor.SetCount( rResData->maRTColorConfig.Count() );	
	for(int idx(0), count(marProxTargetColor.Count()); idx<count; ++idx)
	{
		zcRes::GfxRenderTargetRef rRTColorSurface  = rResData->maRTColorConfig[idx].mrTargetSurface;
		marProxTargetColor[idx] = rRTColorSurface->GetProxy();
		if( marProxTargetColor[idx]->mpTargetColorView )
			mpColorViews[muColorCount++] = marProxTargetColor[idx]->mpTargetColorView;
	}

	//! @todo Support min/max depth rendering
	zenMem::Zero(&mViewport, sizeof(mViewport));		
	mViewport.Width		= (FLOAT)rResData->mvDim.x;
	mViewport.Height	= (FLOAT)rResData->mvDim.y;	
	mViewport.TopLeftX	= (FLOAT)rResData->mvOrigin.x;
	mViewport.TopLeftY	= (FLOAT)rResData->mvOrigin.y;
	mViewport.MinDepth	= 0.0f;
	mViewport.MaxDepth	= 1.0f; 

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