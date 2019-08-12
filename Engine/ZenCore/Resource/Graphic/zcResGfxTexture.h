#pragma once

#include zenHeaderRenderer( zcResGfxTexture )

namespace zcRes
{

class GfxTexture2D : protected GfxTexture2D_HAL
{
zenClassDeclare(GfxTexture2D, GfxTexture2D_HAL)
public:
	
	zenInline const zVec2U16& GetDim()
	{ 
		return maMipData.size() ? maMipData[0].mvDim : zVec2U16::Zero; 
	}
};

}
