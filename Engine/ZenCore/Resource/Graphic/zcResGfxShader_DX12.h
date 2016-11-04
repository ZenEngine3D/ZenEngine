#pragma once
#ifndef __zCore_Res_Gfx_Shaders_DX12_h__
#define __zCore_Res_Gfx_Shaders_DX12_h__
//SF DX12

namespace zcRes
{
	//=================================================================================================
	class GfxShaderVertexHAL_DX11 : public zcExp::ExportGfxShaderDX11
	{
	zenClassDeclare(GfxShaderVertexHAL_DX11, zcExp::ExportGfxShaderDX11)
	public:												
		virtual										~GfxShaderVertexHAL_DX11();												
		bool										Initialize();
		struct ID3D11VertexShader*					mpVertexShader		= nullptr;		
		typedef zcExp::ExporterGfxShaderDX11_DX11	RuntimeExporter;
	};
	class GfxShaderVertexHAL : public GfxShaderVertexHAL_DX11{};

	//=================================================================================================	
	class GfxShaderPixelHAL_DX11 : public zcExp::ExportGfxShaderDX11
	{
	zenClassDeclare(GfxShaderPixelHAL_DX11, zcExp::ExportGfxShaderDX11)
	public:											
		virtual										~GfxShaderPixelHAL_DX11();												
		bool										Initialize();
		struct ID3D11PixelShader*					mpPixelShader		= nullptr;
		typedef zcExp::ExporterGfxShaderDX11_DX11	RuntimeExporter;
	};	
	class GfxShaderPixelHAL : public GfxShaderPixelHAL_DX11{};	
}

#endif