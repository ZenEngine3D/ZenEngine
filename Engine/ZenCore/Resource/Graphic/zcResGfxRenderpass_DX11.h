#pragma once

namespace zcRes
{
	//=============================================================================================
	//! @class	Bind together all the GPU states needed for a drawcall
	//=============================================================================================
	class GfxRenderPass_DX11 : public zcExp::ExportGfxRenderPass
	{
	zenClassDeclare(GfxRenderPass_DX11, zcExp::ExportGfxRenderPass)
	public:
		bool									Initialize();
		GfxStateBlendRef						mrStateBlend;
		GfxStateDepthStencilRef					mrStateDepthStencil;
		GfxStateRasterRef						mrStateRaster;
		GfxViewRef								mrStateView;
		typedef zcExp::ExporterGfxRenderPass	RuntimeExporter;
	};

}
