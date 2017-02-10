#pragma once
#ifndef __zCore_Res_Gfx_State_DX11_h__
#define __zCore_Res_Gfx_State_DX11_h__

namespace zcRes
{
	class GfxStateSampler_DX11 : public zcExp::ExportGfxStateSamplerDX11
	{
	zenClassDeclare(GfxStateSampler_DX11, zcExp::ExportGfxStateSamplerDX11);
	public:
		virtual										~GfxStateSampler_DX11();
		bool										Initialize();
		ID3D11SamplerState*							mpSamplerState;
		typedef zcExp::ExporterGfxStateSamplerDX11_DX11	RuntimeExporter;
	};

	class GfxStateBlend_DX11 : public zcExp::ExportGfxStateBlendDX11
	{
	zenClassDeclare(GfxStateBlend_DX11, zcExp::ExportGfxStateBlendDX11)
	public:
		virtual													~GfxStateBlend_DX11();												
		bool													Initialize();
		ID3D11BlendState*										mpBlendState		= nullptr;
		UINT													muSampleMask		= 0;
		float													mafBlendFactor[4]	= {0,0,0,0}; 
		typedef zcExp::ExporterGfxStateBlendDX11_DX11			RuntimeExporter;
	};
	
	class GfxStateDepthStencil_DX11 : public zcExp::ExportGfxStateDepthStencilDX11
	{
	zenClassDeclare(GfxStateDepthStencil_DX11, zcExp::ExportGfxStateDepthStencilDX11)
	public:
		virtual													~GfxStateDepthStencil_DX11();												
		bool													Initialize();
		ID3D11DepthStencilState*								mpDepthStencilState	= nullptr;
		UINT													muStencilValue		= 0x80;
		typedef zcExp::ExporterGfxStateDepthStencilDX11_DX11	RuntimeExporter;
	};
	
	class GfxStateRaster_DX11 : public zcExp::ExportGfxStateRasterDX11
	{
	zenClassDeclare(GfxStateRaster_DX11, zcExp::ExportGfxStateRasterDX11)
	public:
		virtual													~GfxStateRaster_DX11();												
		bool													Initialize();
		ID3D11RasterizerState*									mpRasterizerState	= nullptr;	
		//bool													mbScreenScissorOn	= false;	//! @todo clean : remove this once proxy is collapsed with owner
		typedef zcExp::ExporterGfxStateRasterDX11_DX11			RuntimeExporter;
	};

}

#endif
