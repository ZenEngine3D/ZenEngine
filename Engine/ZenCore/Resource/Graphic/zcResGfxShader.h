#pragma once
#ifndef __zCore_Res_Gfx_Shaders_h__
#define __zCore_Res_Gfx_Shaders_h__

#include ZENHeaderRenderer(zcResGfxShader)

namespace zcRes
{

//=============================================================================================
//! @class	Empty vertex shader used to match vertex shader signature.
//!			This allow every vertex shader sharing the same input signature, to use the same
//!			ShaderCode when creating a InputLayout, and to not create extra one if they have
//!			the same configuration. 
//!			Also remove the need to keep the vertex shader code loaded, after creating 
//!			it's shader, since the signature contain the relevant part.
//=============================================================================================
class GfxInputSignature : protected GfxInputSignatureHAL_DX11
{
zenClassDeclare(GfxInputSignature, GfxInputSignatureHAL_DX11)
};

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
