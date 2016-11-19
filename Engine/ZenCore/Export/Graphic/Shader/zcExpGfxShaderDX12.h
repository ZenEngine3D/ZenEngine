#pragma once
#if !defined(__zCore_Exp_Gfx_GfxShader_DX12_h__) && ZEN_EXPORT_OR_RESOURCE_DX12
#define __zCore_Exp_Gfx_GfxShader_DX12_h__
//SF DX12

namespace zcExp
{	
	class ExportGfxShaderDX12 : public ExportGfxShader
	{
	zenClassDeclare(ExportGfxShaderDX12, ExportGfxShader)
	public:		
		zArrayStatic<zU8>			maCompiledShader;			//!< Compiled shader code		
	};
}

#endif
