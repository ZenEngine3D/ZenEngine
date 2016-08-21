#pragma once
#ifndef __zCore_Res_Gfx_Shaders_DX11_h__
#define __zCore_Res_Gfx_Shaders_DX11_h__


namespace zcRes
{

//=================================================================================================
	class GfxInputSignatureHAL_DX11 : public zcExp::ExportGfxInputSignatureDX11
	{
	ZENClassDeclare(GfxInputSignatureHAL_DX11, zcExp::ExportGfxInputSignatureDX11)
	public:
		typedef zcExp::ExporterGfxInputSignatureDX11_DX11	RuntimeExporter;
		bool												Initialize();
	};
	class GfxInputSignatureHAL : public GfxInputSignatureHAL_DX11{};
	
	//=================================================================================================
	class GfxShaderVertexHAL_DX11 : public zcExp::ExportGfxShaderDX11
	{
	ZENClassDeclare(GfxShaderVertexHAL_DX11, zcExp::ExportGfxShaderDX11)
	public:												
		virtual										~GfxShaderVertexHAL_DX11();												
		bool										Initialize();
		struct ID3D11VertexShader*					mpVertexShader		= nullptr;		
		GfxInputSignatureRef						mrGfxInputSignature;
		typedef zcExp::ExporterGfxShaderDX11_DX11	RuntimeExporter;
	};
	class GfxShaderVertexHAL : public GfxShaderVertexHAL_DX11{};

	//=================================================================================================	
	class GfxShaderPixelHAL_DX11 : public zcExp::ExportGfxShaderDX11
	{
	ZENClassDeclare(GfxShaderPixelHAL_DX11, zcExp::ExportGfxShaderDX11)
	public:											
		virtual										~GfxShaderPixelHAL_DX11();												
		bool										Initialize();
		struct ID3D11PixelShader*					mpPixelShader		= nullptr;
		typedef zcExp::ExporterGfxShaderDX11_DX11	RuntimeExporter;
	};	
	class GfxShaderPixelHAL : public GfxShaderPixelHAL_DX11{};
}

#endif