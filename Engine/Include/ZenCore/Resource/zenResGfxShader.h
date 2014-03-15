#pragma once
#ifndef __zenCore_ResourceGfxShader_h__
#define __zenCore_ResourceGfxShader_h__

namespace zen { namespace zenRes {	

	//! @todo Clean: remove this from api ?
	AWClassResourceRefDeclare(zGfxInputSignature, zenConst::keResType_GfxInputSignature) };
	AWClassResourceRefDeclare(zGfxShaderParamDef, zenConst::keResType_GfxShaderParamDef) };

	AWClassResourceRefDeclare(zGfxShaderParam, zenConst::keResType_GfxShaderParam)
	public:
	};
	
	AWClassResourceRef2Declare(zGfxShader, zenConst::keResType_GfxShaderVertex, zenConst::keResType_GfxShaderPixel) 
	public:	
	};

	//! @todo Clean: Expose shader compiling to runtime?
	AWClassResourceRefDeclare(zGfxShaderVertex, zenConst::keResType_GfxShaderVertex) 
	public:
		static zGfxShaderVertex Create( const char* _zFilename, const char* _zEntryname );
		static zGfxShaderVertex Create( const char* _zFilename, const char* _zEntryname, const zArrayBase<zShaderDefine>& _aDefines);
	};

	AWClassResourceRefDeclare(zGfxShaderPixel, zenConst::keResType_GfxShaderPixel) 
	public:
		static zGfxShaderPixel Create( const char* _zFilename, const char* _zEntryname );
		static zGfxShaderPixel Create( const char* _zFilename, const char* _zEntryname, const zArrayBase<zShaderDefine>& _aDefines);
	};

}} // namespace zen, zenRes

#endif

