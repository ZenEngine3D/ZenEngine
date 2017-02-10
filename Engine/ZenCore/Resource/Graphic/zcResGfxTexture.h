#pragma once
#ifndef __zCore_Res_Gfx_Texture_h__
#define __zCore_Res_Gfx_Texture_h__

#include zenHeaderRenderer( zcResGfxTexture )

namespace zcRes
{

class GfxTexture2D : protected GfxTexture2D_HAL
{
zenClassDeclare(GfxTexture2D, GfxTexture2D_HAL)
public:
	
	zenInline const zVec2U16& GetDim()
	{ 
		return maMipData.Count() ? maMipData[0].mvDim : zVec2U16::Zero; 
	}
};

}

#endif
