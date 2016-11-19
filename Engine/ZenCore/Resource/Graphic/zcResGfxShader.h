#pragma once
#ifndef __zCore_Res_Gfx_Shaders_h__
#define __zCore_Res_Gfx_Shaders_h__

#include ZENHeaderRenderer(zcResGfxShader)

namespace zcRes
{

class GfxShaderVertex : protected GfxShaderVertexHAL
{
zenClassDeclare(GfxShaderVertex, GfxShaderVertexHAL)
};

class GfxShaderPixel : protected GfxShaderPixelHAL
{
zenClassDeclare(GfxShaderPixel, GfxShaderPixelHAL)
};

}

#endif
