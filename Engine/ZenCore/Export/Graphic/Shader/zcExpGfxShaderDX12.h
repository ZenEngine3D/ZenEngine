#pragma once
#ifndef __zCore_Exp_Gfx_GfxShader_DX12_h__
#define __zCore_Exp_Gfx_GfxShader_DX12_h__

namespace zcExp
{	
	class ExportGfxShaderDX12 : public ExportGfxShader
	{
	zenClassDeclare(ExportGfxShaderDX12, ExportGfxShader)
	public:		
		zArrayStatic<zU8> maCompiledShader;	//!< Compiled shader code		
	};
}

#endif
