#pragma once
#ifndef __zCore_Res_Gfx_Shaders_h__
#define __zCore_Res_Gfx_Shaders_h__

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
class GfxInputSignature : public TResource<GfxInputSignature, GfxInputSignatureExportData, GfxInputSignatureProxy, GfxInputSignatureExporterRuntime>
{
ZENClassDeclare(GfxInputSignature, TResource)
};

//! @todo move shader param infos common object
class GfxShaderVertex : public TResource<GfxShaderVertex, GfxShaderExportData, GfxShaderVertexProxy, GfxShaderExporterRuntime>
{
ZENClassDeclare(GfxShaderVertex, TResource)
public:
	virtual bool								Initialize();
protected:
	GfxInputSignatureRef						mrGfxInputSignature;	//!< Input signature associated with this vertex shader
};

class GfxShaderPixel : public TResource<GfxShaderPixel, GfxShaderExportData, GfxShaderPixelProxy, GfxShaderExporterRuntime>
{
ZENClassDeclare(GfxShaderPixel, TResource)
};

}

#endif