#pragma once
#ifndef __zenApi_Core_ResGfxShader_h__
#define __zenApi_Core_ResGfxShader_h__

namespace zen { namespace zenRes {	

	//! @todo Clean: remove this from api ?
	ZENClassResourceRefDeclare(zGfxInputSignature, zenConst::keResType_GfxInputSignature) };
	ZENClassResourceRefDeclare(zGfxShaderParamDef, zenConst::keResType_GfxShaderParamDef) };

	ZENClassResourceRefDeclare(zGfxShaderParam, zenConst::keResType_GfxShaderParam)
	public:
	};
	
	AWClassResourceRef2Declare(zGfxShader, zenConst::keResType_GfxShaderVertex, zenConst::keResType_GfxShaderPixel) 
	public:	
	};

	//! @todo Clean: Expose shader compiling to runtime?
	ZENClassResourceRefDeclare(zGfxShaderVertex, zenConst::keResType_GfxShaderVertex) 
	public:
		static zGfxShaderVertex Create( const char* _zFilename, const char* _zEntryname );
		static zGfxShaderVertex Create( const char* _zFilename, const char* _zEntryname, const zArrayBase<zShaderDefine>& _aDefines);
	};

	ZENClassResourceRefDeclare(zGfxShaderPixel, zenConst::keResType_GfxShaderPixel) 
	public:
		static zGfxShaderPixel Create( const char* _zFilename, const char* _zEntryname );
		static zGfxShaderPixel Create( const char* _zFilename, const char* _zEntryname, const zArrayBase<zShaderDefine>& _aDefines);
	};

}} // namespace zen, zenRes

#endif

