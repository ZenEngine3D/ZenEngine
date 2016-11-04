#include "zcCore.h"

namespace zcRes
{
		
bool GfxViewHAL_DX11::Initialize()
{
	zenAssert(maRTColorConfig.Count() < D3D11_SIMULTANEOUS_RENDER_TARGET_COUNT);

	GfxTarget2DRef rRTDepthSurface		= mRTDepthConfig.mrTargetSurface;
	mpDepthView							= rRTDepthSurface.IsValid() ? rRTDepthSurface.HAL()->mpTargetDepthView : nullptr;
	muColorCount						= 0;
	zenMem::Zero(mpColorViews, sizeof(mpColorViews));
	for(zUInt idx(0), count(maRTColorConfig.Count()); idx<count; ++idx)
	{
		zcRes::GfxTarget2DRef rRTColorSurface  = maRTColorConfig[idx].mrTargetSurface;
		if( rRTColorSurface.HAL()->mpTargetColorView )
			mpColorViews[muColorCount++] = rRTColorSurface.HAL()->mpTargetColorView;
	}

	//! @todo Support min/max depth rendering
	zenMem::Zero(&mViewport, sizeof(mViewport));		
	mViewport.Width		= (FLOAT)mvDim.x;
	mViewport.Height	= (FLOAT)mvDim.y;	
	mViewport.TopLeftX	= (FLOAT)mvOrigin.x;
	mViewport.TopLeftY	= (FLOAT)mvOrigin.y;
	mViewport.MinDepth	= 0.0f;
	mViewport.MaxDepth	= 1.0f; 

	return true;
}

}