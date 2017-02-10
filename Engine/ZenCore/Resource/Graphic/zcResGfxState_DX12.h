#pragma once
#ifndef __zCore_Res_Gfx_State_DX12_h__
#define __zCore_Res_Gfx_State_DX12_h__
//SF DX12

namespace zcRes
{
	class GfxStateSampler_DX12 : public zcExp::ExportGfxStateSamplerDX12
	{
	zenClassDeclare(GfxStateSampler_DX12, zcExp::ExportGfxStateSamplerDX12);
	public:
		virtual										~GfxStateSampler_DX12();
		bool										Initialize();
		ID3D11SamplerState*							mpSamplerState;
		typedef zcExp::ExporterGfxStateSamplerDX12_DX12	RuntimeExporter;
	};
	
	class GfxStateBlend_DX12 : public zcExp::ExportGfxStateBlendDX12
	{
	zenClassDeclare(GfxStateBlend_DX12, zcExp::ExportGfxStateBlendDX12)
	public:
		virtual													~GfxStateBlend_DX12();												
		bool													Initialize();
		ID3D11BlendState*										mpBlendState		= nullptr;
		UINT													muSampleMask		= 0;
		float													mafBlendFactor[4]	= {0,0,0,0}; 
		typedef zcExp::ExporterGfxStateBlendDX12_DX12			RuntimeExporter;
	};	
	
	class GfxStateDepthStencil_DX12 : public zcExp::ExportGfxStateDepthStencilDX12
	{
	zenClassDeclare(GfxStateDepthStencil_DX12, zcExp::ExportGfxStateDepthStencilDX12)
	public:
		virtual													~GfxStateDepthStencil_DX12();												
		bool													Initialize();
		ID3D11DepthStencilState*								mpDepthStencilState	= nullptr;
		UINT													muStencilValue		= 0x80;
		typedef zcExp::ExporterGfxStateDepthStencilDX12_DX12	RuntimeExporter;
	};
	
	class GfxStateRaster_DX12 : public zcExp::ExportGfxStateRasterDX12
	{
	zenClassDeclare(GfxStateRaster_DX12, zcExp::ExportGfxStateRasterDX12)
	public:
		virtual													~GfxStateRaster_DX12();												
		bool													Initialize();
		ID3D11RasterizerState*									mpRasterizerState	= nullptr;	
		//bool													mbScreenScissorOn	= false;	//! @todo clean : remove this once proxy is collapsed with owner
		typedef zcExp::ExporterGfxStateRasterDX12_DX12			RuntimeExporter;
	};

}

#endif
