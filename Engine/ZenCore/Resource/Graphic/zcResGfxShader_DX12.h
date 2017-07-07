#pragma once

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
