#pragma once

namespace zcExp
{	
	class ExportGfxShaderDX11 : public ExportGfxShader
	{
	zenClassDeclare(ExportGfxShaderDX11, ExportGfxShader)
	public:		
		zArrayDyn<zU8> maCompiledShader;			//!< Compiled shader code		
	};
}
