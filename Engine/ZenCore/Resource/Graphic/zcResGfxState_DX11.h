#pragma once
#ifndef __zCore_Res_Gfx_State_DX11_h__
#define __zCore_Res_Gfx_State_DX11_h__

namespace zcRes
{
	class GfxSamplerHAL_DX11 : public zcExp::ExportGfxSamplerDX11
	{
	zenClassDeclare(GfxSamplerHAL_DX11, zcExp::ExportGfxSamplerDX11);
	public:
		virtual										~GfxSamplerHAL_DX11();
		bool										Initialize();
		ID3D11SamplerState*							mpSamplerState;
		typedef zcExp::ExporterGfxSamplerDX11_DX11	RuntimeExporter;
	};
	class GfxSamplerHAL : public GfxSamplerHAL_DX11{};	

	class GfxStateBlendHAL_DX11 : public zcExp::ExportGfxStateBlendDX11
	{
	zenClassDeclare(GfxStateBlendHAL_DX11, zcExp::ExportGfxStateBlendDX11)
	public:
		virtual													~GfxStateBlendHAL_DX11();												
		bool													Initialize();
		ID3D11BlendState*										mpBlendState		= nullptr;
		UINT													muSampleMask		= 0;
		float													mafBlendFactor[4]	= {0,0,0,0}; 
		typedef zcExp::ExporterGfxStateBlendDX11_DX11			RuntimeExporter;
	};
	class GfxStateBlendHAL : public GfxStateBlendHAL_DX11{};
	
	class GfxStateDepthStencilHAL_DX11 : public zcExp::ExportGfxStateDepthStencilDX11
	{
	zenClassDeclare(GfxStateDepthStencilHAL_DX11, zcExp::ExportGfxStateDepthStencilDX11)
	public:
		virtual													~GfxStateDepthStencilHAL_DX11();												
		bool													Initialize();
		ID3D11DepthStencilState*								mpDepthStencilState	= nullptr;
		UINT													muStencilValue		= 0x80;
		typedef zcExp::ExporterGfxStateDepthStencilDX11_DX11	RuntimeExporter;
	};
	class GfxStateDepthStencilHAL : public GfxStateDepthStencilHAL_DX11{};
	
	class GfxStateRasterHAL_DX11 : public zcExp::ExportGfxStateRasterDX11
	{
	zenClassDeclare(GfxStateRasterHAL_DX11, zcExp::ExportGfxStateRasterDX11)
	public:
		virtual													~GfxStateRasterHAL_DX11();												
		bool													Initialize();
		ID3D11RasterizerState*									mpRasterizerState	= nullptr;	
		//bool													mbScreenScissorOn	= false;	//! @todo clean : remove this once proxy is collapsed with owner
		typedef zcExp::ExporterGfxStateRasterDX11_DX11			RuntimeExporter;
	};
	class GfxStateRasterHAL : public GfxStateRasterHAL_DX11{};
}

#endif
