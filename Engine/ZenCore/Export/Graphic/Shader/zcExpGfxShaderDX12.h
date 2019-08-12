#pragma once

namespace zcExp
{	
	class ExportGfxShaderDX12 : public ExportGfxShader
	{
	zenClassDeclare(ExportGfxShaderDX12, ExportGfxShader)
	public:		
		zArrayDyn<zU8> maCompiledShader;	//!< Compiled shader code		
	};
}
