#pragma once
#ifndef __zCore_Res_Gfx_GfxView_h__
#define __zCore_Res_Gfx_GfxView_h__

#include zenHeaderRenderer(zcResGfxView)

namespace zcRes
{

class GfxView : protected GfxView_HAL
{
zenClassDeclare(GfxView, GfxView_HAL)
public:
	const zVec2U16&	GetDim()const;
};

}

#endif