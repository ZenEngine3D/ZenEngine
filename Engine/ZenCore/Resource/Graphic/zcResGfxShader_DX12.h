#pragma once
#ifndef __zCore_Res_Gfx_Shaders_DX12_h__
#define __zCore_Res_Gfx_Shaders_DX12_h__
//SF DX12

namespace zcRes
{
	//=================================================================================================
	class GfxShaderVertexHAL_DX12 : public zcExp::ExportGfxShaderDX12
	{
	zenClassDeclare(GfxShaderVertexHAL_DX12, zcExp::ExportGfxShaderDX12)
	public:												
		virtual										~GfxShaderVertexHAL_DX12();												
		bool										Initialize();
		struct ID3D11VertexShader*					mpVertexShader		= nullptr;		
		typedef zcExp::ExporterGfxShaderDX12_DX12	RuntimeExporter;
	};
	class GfxShaderVertexHAL : public GfxShaderVertexHAL_DX12{};

	//=================================================================================================	
	class GfxShaderPixelHAL_DX12 : public zcExp::ExportGfxShaderDX12
	{
	zenClassDeclare(GfxShaderPixelHAL_DX12, zcExp::ExportGfxShaderDX12)
	public:											
		virtual										~GfxShaderPixelHAL_DX12();												
		bool										Initialize();
		struct ID3D11PixelShader*					mpPixelShader		= nullptr;
		typedef zcExp::ExporterGfxShaderDX12_DX12	RuntimeExporter;
	};	
	class GfxShaderPixelHAL : public GfxShaderPixelHAL_DX12{};	
}

#endif