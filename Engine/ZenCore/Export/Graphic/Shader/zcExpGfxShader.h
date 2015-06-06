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

	zResID CreateGfxShaderVertex( const char* _zFilename, const char* _zEntryname );
	zResID CreateGfxShaderPixel	( const char* _zFilename, const char* _zEntryname );
	zResID CreateGfxShaderVertex( const char* _zFilename, const char* _zEntryname, const zArrayBase<zenRes::zShaderDefine>& _aDefines );
	zResID CreateGfxShaderPixel	( const char* _zFilename, const char* _zEntryname, const zArrayBase<zenRes::zShaderDefine>& _aDefines );
}

#include "zcExpGfxShaderDX11.h"

#endif