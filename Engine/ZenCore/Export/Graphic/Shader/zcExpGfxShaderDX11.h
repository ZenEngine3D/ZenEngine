#pragma once

namespace zcExp
{	
	class ExportGfxShaderDX11 : public ExportGfxShader
	{
	zenClassDeclare(ExportGfxShaderDX11, ExportGfxShader)
	public:		
		zArrayStatic<zU8>			maCompiledShader;			//!< Compiled shader code		
	};
}
