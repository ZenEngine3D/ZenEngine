#pragma once
#ifndef __zCore_Exp_Gfx_Shader_h__
#define __zCore_Exp_Gfx_Shader_h__

namespace zcExp
{
	struct ExportInfoGfxShader : public ExportInfoBase
	{				
		static zResID						CallbackGetItemID(zenConst::eResPlatform _ePlatform, zenConst::eResType _eType, zenConst::eResSource _eSource, const zcExp::ExportInfoBase* _pExportInfo, bool& _bExistOut);
		zString								mzFilename;
		zString								mzEntryname;
		zArrayStatic<zenRes::zShaderDefine>	maDefines;
		zenConst::eShaderStage				meShaderStage;
	};

	class ExportGfxShader : public zenRes::zExportData
	{
	ZENClassDeclare(ExportGfxShader, zenRes::zExportData)
	public:
		//virtual bool				Serialize( zcExp::Serializer_Base& _Serializer ){return FALSE;}
		struct BindInfo{ zU8 uSlot;	zU8 uCount;	};
																//!< @todo feature support buffer slots
		zenConst::eShaderStage		meShaderStage;				//!< Type of shader (vertex, pixel, ...)
		zArrayStatic<zResID>		maParamDefID;				//!< List of Parameter info used with this shader
		zArrayStatic<zHash32>		maTextureSamplerName;		//!< List of name used for Texture and associated Sampler 
		zArrayStatic<BindInfo>		maTextureSamplerSlot;		//!< Texture or sampler slot used for each item in mTextureSamplerName
		zResID						mShaderInputSignatureID;	//!< Used for vertex shader
		zU8							muTextureSlotCount;			//!< Texture slot used in shader
	};

	zResID CreateGfxShaderVertex( const char* _zFilename, const char* _zEntryname );
	zResID CreateGfxShaderPixel	( const char* _zFilename, const char* _zEntryname );
	zResID CreateGfxShaderVertex( const char* _zFilename, const char* _zEntryname, const zArrayBase<zenRes::zShaderDefine>& _aDefines );
	zResID CreateGfxShaderPixel	( const char* _zFilename, const char* _zEntryname, const zArrayBase<zenRes::zShaderDefine>& _aDefines );
}

#include "zcExpGfxShaderDX11.h"

#endif