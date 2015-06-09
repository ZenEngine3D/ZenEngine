#pragma once
#if !defined(__zCore_Exp_Gfx_ShaderParamDef_DX11_h__) && ZEN_EXPORT_OR_RESOURCE_DX11
#define __zCore_Exp_Gfx_ShaderParamDef_DX11_h__

namespace zcExp
{
	class ResDataGfxShaderParamDefDX11 : public ResourceData
	{
	ZENClassDeclare(ResDataGfxShaderParamDefDX11, ResourceData)
	public:
		virtual bool			Serialize( zcExp::Serializer_Base& _Serializer ){return true;}				
		eShaderParamFreq		meFrequence;		
		ShaderParamItemInfoMap	mdParameters;
		zArrayStatic<zU8>		maParameterDefaults;
	};
}

#endif
