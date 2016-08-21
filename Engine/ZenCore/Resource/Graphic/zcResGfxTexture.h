#pragma once
#ifndef __zCore_Res_Gfx_Texture_h__
#define __zCore_Res_Gfx_Texture_h__

namespace zcRes
{

#include ZENHeaderRenderer(zcResGfxTexture)

class GfxTexture2d : protected GfxTexture2dHAL
{
ZENClassDeclare(GfxTexture2d, GfxTexture2dHAL)
public:
	
	ZENInline const zVec2U16& GetDim()
	{ 
		return maMipData.Count() ? maMipData[0].mvDim : zVec2U16::Zero; 
	}
};

}

#endif
