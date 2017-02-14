#pragma once

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
		zenInline bool											Initialize(){return true;}
		typedef zcExp::ExporterGfxStateBlendDX12_DX12			RuntimeExporter;
	};	
	
	class GfxStateDepthStencil_DX12 : public zcExp::ExportGfxStateDepthStencilDX12
	{
	zenClassDeclare(GfxStateDepthStencil_DX12, zcExp::ExportGfxStateDepthStencilDX12)
	public:
		zenInline bool											Initialize(){return true;}
		UINT													muStencilValue		= 0x80;	//! @todo 3 Support stencil on all platforms
		typedef zcExp::ExporterGfxStateDepthStencilDX12_DX12	RuntimeExporter;
	};
	
	class GfxStateRaster_DX12 : public zcExp::ExportGfxStateRasterDX12
	{
	zenClassDeclare(GfxStateRaster_DX12, zcExp::ExportGfxStateRasterDX12)
	public:
		zenInline bool											Initialize(){return true;}
		typedef zcExp::ExporterGfxStateRasterDX12_DX12			RuntimeExporter;
	};

}
