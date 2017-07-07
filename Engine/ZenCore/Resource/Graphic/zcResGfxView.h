#pragma once

#include zenHeaderRenderer(zcResGfxView)

namespace zcRes
{

class GfxView : protected GfxView_HAL
{
zenClassDeclare(GfxView, GfxView_HAL)
public:
	const zVec2U16&	GetDim()const;	
	const zenRes::zGfxTarget2D& GetColorTarget(zUInt _uTargetIndex=0)const;
	const zenRes::zGfxTarget2D& GetDepthTarget()const;
};

}
