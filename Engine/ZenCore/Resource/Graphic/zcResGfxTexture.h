#pragma once
#ifndef __zCore_Res_Gfx_Texture_h__
#define __zCore_Res_Gfx_Texture_h__

#include ZENHeaderRenderer( zcResGfxTexture )

namespace zcRes
{

class GfxTexture2d : protected GfxTexture2dHAL
{
zenClassDeclare(GfxTexture2d, GfxTexture2dHAL)
public:
	
	zenInline const zVec2U16& GetDim()
	{ 
		return maMipData.Count() ? maMipData[0].mvDim : zVec2U16::Zero; 
	}
};

}

#endif
