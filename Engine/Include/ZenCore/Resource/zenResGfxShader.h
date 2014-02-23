#pragma once
#ifndef __zenCore_ResourceGfxShader_h__
#define __zenCore_ResourceGfxShader_h__

namespace zen { namespace awres {	

	//! @todo remove this from api ?
	AWClassResourceRefDeclare(awGfxInputSignature, awconst::keResType_GfxInputSignature) };
	AWClassResourceRefDeclare(awGfxShaderParamDef, awconst::keResType_GfxShaderParamDef) };

	AWClassResourceRefDeclare(awGfxShaderParam, awconst::keResType_GfxShaderParam)
	public:
	};
	
	AWClassResourceRef2Declare(awGfxShader, awconst::keResType_GfxShaderVertex, awconst::keResType_GfxShaderPixel) 
	public:	
	};

	//! @todo Expose shader compiling to runtime?
	AWClassResourceRefDeclare(awGfxShaderVertex, awconst::keResType_GfxShaderVertex) 
	public:
		static awGfxShaderVertex Create( const char* _zFilename, const char* _zEntryname );
		static awGfxShaderVertex Create( const char* _zFilename, const char* _zEntryname, const zenArrayBase<awShaderDefine>& _aDefines);
	};

	AWClassResourceRefDeclare(awGfxShaderPixel, awconst::keResType_GfxShaderPixel) 
	public:
		static awGfxShaderPixel Create( const char* _zFilename, const char* _zEntryname );
		static awGfxShaderPixel Create( const char* _zFilename, const char* _zEntryname, const zenArrayBase<awShaderDefine>& _aDefines);
	};

}} // namespace zen, awres

#endif

