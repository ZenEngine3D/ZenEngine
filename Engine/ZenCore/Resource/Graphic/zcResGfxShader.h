#pragma once
#ifndef __zCore_Res_Gfx_Shaders_h__
#define __zCore_Res_Gfx_Shaders_h__

#include zenHeaderRenderer(zcResGfxShader)

namespace zcRes
{

class GfxShaderVertex : protected GfxShaderVertex_HAL
{
zenClassDeclare(GfxShaderVertex, GfxShaderVertex_HAL)
};

class GfxShaderPixel : protected GfxShaderPixel_HAL
{
zenClassDeclare(GfxShaderPixel, GfxShaderPixel_HAL)
};

}

#endif
