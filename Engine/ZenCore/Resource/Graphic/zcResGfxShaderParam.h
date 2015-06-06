#pragma once
#ifndef __zCore_Res_Gfx_GfxShaderParam_h__
#define __zCore_Res_Gfx_GfxShaderParam_h__

namespace zcRes
{
	class GfxShaderParamDef : public TResource<GfxShaderParamDef, GfxShaderParamDefResData, GfxShaderParamDefProxy, GfxShaderParamDefExporterRuntime>
	{
	ZENClassDeclare(GfxShaderParamDef, TResource)
	public:
	};

	class GfxShaderParam : public TResource<GfxShaderParam, GfxShaderParamResData, GfxShaderParamProxy, GfxShaderParamExporterRuntime>
	{
	ZENClassDeclare(GfxShaderParam, TResource)
	public:
	};	
}

#endif
