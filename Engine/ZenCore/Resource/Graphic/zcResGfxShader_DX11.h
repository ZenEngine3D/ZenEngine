#pragma once
#ifndef __zCore_Res_Gfx_Shaders_DX11_h__
#define __zCore_Res_Gfx_Shaders_DX11_h__


namespace zcRes
{
	//=================================================================================================
	class GfxShaderVertex_DX11 : public zcExp::ExportGfxShaderDX11
	{
	zenClassDeclare(GfxShaderVertex_DX11, zcExp::ExportGfxShaderDX11)
	public:												
		virtual										~GfxShaderVertex_DX11();												
		bool										Initialize();
		struct ID3D11VertexShader*					mpVertexShader		= nullptr;		
		typedef zcExp::ExporterGfxShaderDX11_DX11	RuntimeExporter;
	};

	//=================================================================================================	
	class GfxShaderPixel_DX11 : public zcExp::ExportGfxShaderDX11
	{
	zenClassDeclare(GfxShaderPixel_DX11, zcExp::ExportGfxShaderDX11)
	public:											
		virtual										~GfxShaderPixel_DX11();												
		bool										Initialize();
		struct ID3D11PixelShader*					mpPixelShader		= nullptr;
		typedef zcExp::ExporterGfxShaderDX11_DX11	RuntimeExporter;
	};
}

#endif