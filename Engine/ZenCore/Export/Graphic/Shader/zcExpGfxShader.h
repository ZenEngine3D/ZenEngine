#pragma once
#ifndef __zCore_Exp_Gfx_Shader_h__
#define __zCore_Exp_Gfx_Shader_h__

namespace zcExp
{
	class SerialShader_Base : public zcExp::ExportItem
	{
	ZENClassDeclare(SerialShader_Base, zcExp::ExportItem)
	public:
		struct ExportInfo : public zcExp::ExportInfoBase
		{				
			zString								mzFilename;
			zString								mzEntryname;
			zArrayStatic<zenRes::zShaderDefine>	maDefines;
			zcExp::eShaderStage						meShaderStage;
		};

		struct BindInfo{ zU8 uSlot;	zU8 uCount;	};
		eShaderStage								meShaderStage;			//!< Type of shader (vertex, pixel, ...)
		zArrayStatic<zResID>					maParamDefID;			//!< List of Parameter info used with this shader
		zArrayStatic<zHash32>						maTextureSamplerName;	//!< List of name used for Texture and associated Sampler 
		zArrayStatic<BindInfo>						maTextureSamplerSlot;	//!< Texture or sampler slot used for each item in mTextureSamplerName
		zU8										muTextureSlotCount;		//!< Texture slot used in shader

		static zResID CallbackGetItemID(zenConst::eResPlatform _ePlatform, zenConst::eResType _eType, zenConst::eResSource _eSource, const zcExp::ExportInfoBase* _pExportInfo, bool& _bExistOut);
	};
	
	zResID CreateGfxShaderVertex	( const char* _zFilename, const char* _zEntryname );
	zResID CreateGfxShaderPixel	( const char* _zFilename, const char* _zEntryname );
	zResID CreateGfxShaderVertex	( const char* _zFilename, const char* _zEntryname, const zArrayBase<zenRes::zShaderDefine>& _aDefines );
	zResID CreateGfxShaderPixel	( const char* _zFilename, const char* _zEntryname, const zArrayBase<zenRes::zShaderDefine>& _aDefines );
}

#include "zcExpGfxShaderDX11.h"

#endif