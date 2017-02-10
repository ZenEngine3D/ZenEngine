#pragma once
#ifndef __zCore_Res_Gfx_Shaders_DX12_h__
#define __zCore_Res_Gfx_Shaders_DX12_h__

namespace zcRes
{
	//=================================================================================================
	class GfxShaderVertex_DX12 : public zcExp::ExportGfxShaderDX12
	{
	zenClassDeclare(GfxShaderVertex_DX12, zcExp::ExportGfxShaderDX12)
	public:												
		virtual										~GfxShaderVertex_DX12();												
		bool										Initialize();
		D3D12_SHADER_BYTECODE						mDXShaderCode;
		typedef zcExp::ExporterGfxShaderDX12_DX12	RuntimeExporter;
	};

	//=================================================================================================	
	class GfxShaderPixel_DX12 : public zcExp::ExportGfxShaderDX12
	{
	zenClassDeclare(GfxShaderPixel_DX12, zcExp::ExportGfxShaderDX12)
	public:											
		virtual										~GfxShaderPixel_DX12();												
		bool										Initialize();
		D3D12_SHADER_BYTECODE						mDXShaderCode;
		typedef zcExp::ExporterGfxShaderDX12_DX12	RuntimeExporter;
	};	
}

#endif
