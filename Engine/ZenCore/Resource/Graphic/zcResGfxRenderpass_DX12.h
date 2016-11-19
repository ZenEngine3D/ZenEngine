#pragma once
#ifndef __zCore_Res_Gfx_Renderpass_DX12_h__
#define __zCore_Res_Gfx_Renderpass_DX12_h__
//SF DX12

namespace zcRes
{
	//=============================================================================================
	//! @class	Bind together all the GPU states needed for a drawcall
	//=============================================================================================
	class GfxRenderPassHAL_DX12 : public zcExp::ExportGfxRenderPass
	{
	zenClassDeclare(GfxRenderPassHAL_DX12, zcExp::ExportGfxRenderPass)
	public:
		bool									Initialize();
		GfxStateBlendRef						mrStateBlend;
		GfxStateDepthStencilRef					mrStateDepthStencil;
		GfxStateRasterRef						mrStateRaster;
		GfxViewRef								mrStateView;
		typedef zcExp::ExporterGfxRenderPass	RuntimeExporter;
	};
	class GfxRenderPassHAL : public GfxRenderPassHAL_DX12{};
}

#endif
