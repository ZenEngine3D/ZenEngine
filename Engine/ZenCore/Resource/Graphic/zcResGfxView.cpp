#include "zcCore.h"

namespace zcRes
{

const zVec2U16& GfxView::GetDim()const
{
	return mvDim;
}

const zenRes::zGfxTarget2D& GfxView::GetColorTarget(zUInt _uTargetIndex)const
{
	static zenRes::zGfxTarget2D srNullTarget2D;
	return (_uTargetIndex < maRTColorConfig.Count()) ? maRTColorConfig[_uTargetIndex].mrTargetSurface : srNullTarget2D;
}

const zenRes::zGfxTarget2D& GfxView::GetDepthTarget()const
{
	return mRTDepthConfig.mrTargetSurface;
}

}