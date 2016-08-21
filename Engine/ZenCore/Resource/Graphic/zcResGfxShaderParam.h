#pragma once
#ifndef __zCore_Res_Gfx_GfxShaderParam_h__
#define __zCore_Res_Gfx_GfxShaderParam_h__

namespace zcRes
{
	class GfxShaderParamDef : protected GfxShaderParamDefHAL
	{
	ZENClassDeclare(GfxShaderParamDef, GfxShaderParamDefHAL)
	public:
	};

	class GfxShaderParam : protected GfxShaderParamHAL
	{
	ZENClassDeclare(GfxShaderParam, GfxShaderParamHAL)
	public:
		using Super::SetValue;
	};	
}

#endif
