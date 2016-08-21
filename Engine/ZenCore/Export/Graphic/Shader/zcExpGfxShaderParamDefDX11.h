#pragma once
#if !defined(__zCore_Exp_Gfx_ShaderParamDef_DX11_h__) && ZEN_EXPORT_OR_RESOURCE_DX11
#define __zCore_Exp_Gfx_ShaderParamDef_DX11_h__

namespace zcExp
{
	class ExportGfxShaderParamDefDX11 : public zenRes::zExportData
	{
	ZENClassDeclare(ExportGfxShaderParamDefDX11, zenRes::zExportData)
	public:
		eShaderParamFreq		meFrequence;
		ShaderParamItemInfoMap	mdParameters;
		zArrayStatic<zU8>		maParameterDefaults;
	};
}

#endif
