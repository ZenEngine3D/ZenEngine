#include "zcCore.h"

//SF DX12
namespace zcRes
{
		
bool GfxView_DX12::Initialize()
{
	zenAssert(maRTColorConfig.size() < D3D12_SIMULTANEOUS_RENDER_TARGET_COUNT);	
	/*
	GfxTarget2DRef rRTDepthSurface		= mRTDepthConfig.mrTargetSurface;
	//! @todo 1 support null view
//	mDepthView							= rRTDepthSurface.IsValid() ? rRTDepthSurface.HAL()->mTargetDepthView : nullptr;
	mDepthView							= rRTDepthSurface.HAL()->mTargetDepthView.GetCpu();
	muColorCount						= 0;	
	for(zUInt idx(0), count(maRTColorConfig.size()); idx<count; ++idx)
	{
		const GfxTarget2D_HAL* const pTarget	= maRTColorConfig[idx].mrTargetSurface.HAL();
		//mColorViews[idx]						= pTarget->mTargetColorView.IsValid() ? pTarget->mTargetColorView.GetCpu() : nullptr;
		maColorViews[idx]						= pTarget->mTargetColorView.GetCpu();
		muColorCount							+= pTarget->mTargetColorView.IsValid();
	}
	*/
	mViewport							= CD3DX12_VIEWPORT(mvOrigin.x, mvOrigin.y, mvDim.x, mvDim.y);
	return true;
}

}