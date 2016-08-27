#pragma once
#if !defined(__zCore_Exp_Gfx_GfxShader_DX11_h__) && ZEN_EXPORT_OR_RESOURCE_DX11
#define __zCore_Exp_Gfx_GfxShader_DX11_h__

namespace zcExp
{	
	class ExportGfxShaderDX11 : public ExportGfxShader
	{
	zenClassDeclare(ExportGfxShaderDX11, ExportGfxShader)
	public:		
		zArrayStatic<zU8>			maCompiledShader;			//!< Compiled shader code		
	};
}

#endif
