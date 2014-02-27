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
			zenString								mzFilename;
			zenString								mzEntryname;
			zenArrayStatic<awres::awShaderDefine>	maDefines;
			EExp::eShaderStage						meShaderStage;
		};

		struct BindInfo{ zenU8 uSlot;	zenU8 uCount;	};
		eShaderStage								meShaderStage;			//!< Type of shader (vertex, pixel, ...)
		zenArrayStatic<zenResID>					maParamDefID;			//!< List of Parameter info used with this shader
		zenArrayStatic<zenHash32>						maTextureSamplerName;	//!< List of name used for Texture and associated Sampler 
		zenArrayStatic<BindInfo>						maTextureSamplerSlot;	//!< Texture or sampler slot used for each item in mTextureSamplerName
		zenU8										muTextureSlotCount;		//!< Texture slot used in shader

		static zenResID CallbackGetItemID(awconst::eResPlatform _ePlatform, awconst::eResType _eType, awconst::eResSource _eSource, const EExp::ExportInfoBase* _pExportInfo, bool& _bExistOut);
	};
	
	zenResID CreateGfxShaderVertex	( const char* _zFilename, const char* _zEntryname );
	zenResID CreateGfxShaderPixel	( const char* _zFilename, const char* _zEntryname );
	zenResID CreateGfxShaderVertex	( const char* _zFilename, const char* _zEntryname, const zenArrayBase<awres::awShaderDefine>& _aDefines );
	zenResID CreateGfxShaderPixel	( const char* _zFilename, const char* _zEntryname, const zenArrayBase<awres::awShaderDefine>& _aDefines );
}

#include "EExpShaderDX11.h"

#endif