#pragma once
#ifndef __zCore_Res_Gfx_Shaders_h__
#define __zCore_Res_Gfx_Shaders_h__

#include ZENHeaderRenderer(zcResGfxShader)

namespace zcRes
{

class GfxShaderVertex : protected GfxShaderVertexHAL_DX11
{
zenClassDeclare(GfxShaderVertex, GfxShaderVertexHAL_DX11)
};

class GfxShaderPixel : protected GfxShaderPixelHAL_DX11
{
zenClassDeclare(GfxShaderPixel, GfxShaderPixelHAL_DX11)
};

}

#endif
