#pragma once
#ifndef __LibExport_Graphics_Shader_h__
#define __LibExport_Graphics_Shader_h__

namespace EExp
{
	class SerialShader_Base : public EExp::ExportItem
	{
	AWClassDeclare(SerialShader_Base, EExp::ExportItem)
	public:
		struct ExportInfo : public EExp::ExportInfoBase
		{				
			awString								mzFilename;
			awString								mzEntryname;
			awArrayStatic<awres::awShaderDefine>	maDefines;
			EExp::eShaderStage						meShaderStage;
		};

		struct BindInfo{ awU8 uSlot;	awU8 uCount;	};
		eShaderStage								meShaderStage;			//!< Type of shader (vertex, pixel, ...)
		awArrayStatic<awResourceID>					maParamDefID;			//!< List of Parameter info used with this shader
		awArrayStatic<awHash32>						maTextureSamplerName;	//!< List of name used for Texture and associated Sampler 
		awArrayStatic<BindInfo>						maTextureSamplerSlot;	//!< Texture or sampler slot used for each item in mTextureSamplerName
		awU8										muTextureSlotCount;		//!< Texture slot used in shader

		static awResourceID CallbackGetItemID(awconst::eResPlatform _ePlatform, awconst::eResType _eType, awconst::eResSource _eSource, const EExp::ExportInfoBase* _pExportInfo, bool& _bExistOut);
	};
	
	awResourceID CreateGfxShaderVertex	( const char* _zFilename, const char* _zEntryname );
	awResourceID CreateGfxShaderPixel	( const char* _zFilename, const char* _zEntryname );
	awResourceID CreateGfxShaderVertex	( const char* _zFilename, const char* _zEntryname, const awArrayBase<awres::awShaderDefine>& _aDefines );
	awResourceID CreateGfxShaderPixel	( const char* _zFilename, const char* _zEntryname, const awArrayBase<awres::awShaderDefine>& _aDefines );
}

#include "EExpShaderDX11.h"

#endif