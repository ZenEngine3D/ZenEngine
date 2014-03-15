#pragma once
#ifndef __LibExport_Graphics_Shader_h__
#define __LibExport_Graphics_Shader_h__

namespace EExp
{
	class SerialShader_Base : public EExp::ExportItem
	{
	ZENClassDeclare(SerialShader_Base, EExp::ExportItem)
	public:
		struct ExportInfo : public EExp::ExportInfoBase
		{				
			zString								mzFilename;
			zString								mzEntryname;
			zArrayStatic<zenRes::zShaderDefine>	maDefines;
			EExp::eShaderStage						meShaderStage;
		};

		struct BindInfo{ zU8 uSlot;	zU8 uCount;	};
		eShaderStage								meShaderStage;			//!< Type of shader (vertex, pixel, ...)
		zArrayStatic<zResID>					maParamDefID;			//!< List of Parameter info used with this shader
		zArrayStatic<zHash32>						maTextureSamplerName;	//!< List of name used for Texture and associated Sampler 
		zArrayStatic<BindInfo>						maTextureSamplerSlot;	//!< Texture or sampler slot used for each item in mTextureSamplerName
		zU8										muTextureSlotCount;		//!< Texture slot used in shader

		static zResID CallbackGetItemID(zenConst::eResPlatform _ePlatform, zenConst::eResType _eType, zenConst::eResSource _eSource, const EExp::ExportInfoBase* _pExportInfo, bool& _bExistOut);
	};
	
	zResID CreateGfxShaderVertex	( const char* _zFilename, const char* _zEntryname );
	zResID CreateGfxShaderPixel	( const char* _zFilename, const char* _zEntryname );
	zResID CreateGfxShaderVertex	( const char* _zFilename, const char* _zEntryname, const zArrayBase<zenRes::zShaderDefine>& _aDefines );
	zResID CreateGfxShaderPixel	( const char* _zFilename, const char* _zEntryname, const zArrayBase<zenRes::zShaderDefine>& _aDefines );
}

#include "EExpShaderDX11.h"

#endif