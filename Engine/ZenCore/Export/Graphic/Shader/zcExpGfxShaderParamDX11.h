#pragma once
#if !defined(__zCore_Exp_GfxShaderParamDX11_h__) && ZEN_EXPORT_OR_RESOURCE_DX11
#define __zCore_Exp_GfxShaderParamDX11_h__

namespace zcExp
{
	class ResDataGfxShaderParamDX11 : public ResourceData
	{
	ZENClassDeclare(ResDataGfxShaderParamDX11, ResourceData)
	public:
		virtual bool			Serialize( zcExp::Serializer_Base& _Serializer ){return true;}
		zResID					mParentParamDefID;	//!< Parent ShaderParam definition
		zArrayStatic<zU8>		maParameterValues;	//!< Bytes data for all buffer values (see ShaderParamDef for each parameter start offset in array)
	};
}

#endif
