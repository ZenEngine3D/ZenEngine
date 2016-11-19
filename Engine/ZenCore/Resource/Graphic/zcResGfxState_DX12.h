#pragma once
#ifndef __zCore_Res_Gfx_State_DX12_h__
#define __zCore_Res_Gfx_State_DX12_h__
//SF DX12

namespace zcRes
{
	class GfxSamplerHAL_DX12 : public zcExp::ExportGfxSamplerDX12
	{
	zenClassDeclare(GfxSamplerHAL_DX12, zcExp::ExportGfxSamplerDX12);
	public:
		virtual										~GfxSamplerHAL_DX12();
		bool										Initialize();
		ID3D11SamplerState*							mpSamplerState;
		typedef zcExp::ExporterGfxSamplerDX12_DX12	RuntimeExporter;
	};
	class GfxSamplerHAL : public GfxSamplerHAL_DX12{};	

	class GfxStateBlendHAL_DX12 : public zcExp::ExportGfxStateBlendDX12
	{
	zenClassDeclare(GfxStateBlendHAL_DX12, zcExp::ExportGfxStateBlendDX12)
	public:
		virtual													~GfxStateBlendHAL_DX12();												
		bool													Initialize();
		ID3D11BlendState*										mpBlendState		= nullptr;
		UINT													muSampleMask		= 0;
		float													mafBlendFactor[4]	= {0,0,0,0}; 
		typedef zcExp::ExporterGfxStateBlendDX12_DX12			RuntimeExporter;
	};
	class GfxStateBlendHAL : public GfxStateBlendHAL_DX12{};
	
	class GfxStateDepthStencilHAL_DX12 : public zcExp::ExportGfxStateDepthStencilDX12
	{
	zenClassDeclare(GfxStateDepthStencilHAL_DX12, zcExp::ExportGfxStateDepthStencilDX12)
	public:
		virtual													~GfxStateDepthStencilHAL_DX12();												
		bool													Initialize();
		ID3D11DepthStencilState*								mpDepthStencilState	= nullptr;
		UINT													muStencilValue		= 0x80;
		typedef zcExp::ExporterGfxStateDepthStencilDX12_DX12	RuntimeExporter;
	};
	class GfxStateDepthStencilHAL : public GfxStateDepthStencilHAL_DX12{};
	
	class GfxStateRasterHAL_DX12 : public zcExp::ExportGfxStateRasterDX12
	{
	zenClassDeclare(GfxStateRasterHAL_DX12, zcExp::ExportGfxStateRasterDX12)
	public:
		virtual													~GfxStateRasterHAL_DX12();												
		bool													Initialize();
		ID3D11RasterizerState*									mpRasterizerState	= nullptr;	
		//bool													mbScreenScissorOn	= false;	//! @todo clean : remove this once proxy is collapsed with owner
		typedef zcExp::ExporterGfxStateRasterDX12_DX12			RuntimeExporter;
	};
	class GfxStateRasterHAL : public GfxStateRasterHAL_DX12{};
}

#endif
