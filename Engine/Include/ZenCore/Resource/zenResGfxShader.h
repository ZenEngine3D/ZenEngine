#pragma once
#ifndef __zenCore_ResourceGfxShader_h__
#define __zenCore_ResourceGfxShader_h__

namespace zen { namespace zenRes {	

	//! @todo remove this from api ?
	AWClassResourceRefDeclare(GfxInputSignature, awconst::keResType_GfxInputSignature) };
	AWClassResourceRefDeclare(GfxShaderParamDef, awconst::keResType_GfxShaderParamDef) };

	AWClassResourceRefDeclare(GfxShaderParam, awconst::keResType_GfxShaderParam)
	public:
	};
	
	AWClassResourceRef2Declare(GfxShader, awconst::keResType_GfxShaderVertex, awconst::keResType_GfxShaderPixel) 
	public:	
	};

	//! @todo Expose shader compiling to runtime?
	AWClassResourceRefDeclare(GfxShaderVertex, awconst::keResType_GfxShaderVertex) 
	public:
		static GfxShaderVertex Create( const char* _zFilename, const char* _zEntryname );
		static GfxShaderVertex Create( const char* _zFilename, const char* _zEntryname, const zenArrayBase<awShaderDefine>& _aDefines);
	};

	AWClassResourceRefDeclare(GfxShaderPixel, awconst::keResType_GfxShaderPixel) 
	public:
		static GfxShaderPixel Create( const char* _zFilename, const char* _zEntryname );
		static GfxShaderPixel Create( const char* _zFilename, const char* _zEntryname, const zenArrayBase<awShaderDefine>& _aDefines);
	};

}} // namespace zen, zenRes

#endif

