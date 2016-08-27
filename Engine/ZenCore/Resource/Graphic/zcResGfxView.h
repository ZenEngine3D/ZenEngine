#pragma once
#ifndef __zCore_Res_Gfx_GfxView_h__
#define __zCore_Res_Gfx_GfxView_h__

#include ZENHeaderRenderer(zcResGfxView)

namespace zcRes
{

class GfxView : protected GfxViewHAL
{
zenClassDeclare(GfxView, GfxViewHAL)
public:
	const zVec2U16&	GetDim()const;
};

}

#endif