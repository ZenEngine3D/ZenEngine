#pragma once
#if !defined(__zCore_Exp_Gfx_GfxShader_DX11_h__) && ZEN_EXPORT_OR_RESOURCE_DX11
#define __zCore_Exp_Gfx_GfxShader_DX11_h__

namespace zcExp
{	
	class ExportDataGfxShaderDX11 : public ExportDataBase
	{
	ZENClassDeclare(ExportDataGfxShaderDX11, ExportDataBase)
	public:
		virtual bool				Serialize( zcExp::Serializer_Base& _Serializer );
		struct BindInfo{ zU8 uSlot;	zU8 uCount;	};
		zenConst::eShaderStage		meShaderStage;				//!< Type of shader (vertex, pixel, ...)
		zArrayStatic<zResID>		maParamDefID;				//!< List of Parameter info used with this shader
		zArrayStatic<zHash32>		maTextureSamplerName;		//!< List of name used for Texture and associated Sampler 
		zArrayStatic<BindInfo>		maTextureSamplerSlot;		//!< Texture or sampler slot used for each item in mTextureSamplerName
		zResID						mShaderInputSignatureID;	//!< Used for vertex shader
		zArrayStatic<zU8>			maCompiledShader;			//!< Compiled shader code
		zU8							muTextureSlotCount;			//!< Texture slot used in shader
	};
}

#endif