#pragma once
#ifndef __zCore_Res_Gfx_State_h__
#define __zCore_Res_Gfx_State_h__

namespace zcRes
{
	class GfxSampler : public TResource<GfxSampler, GfxSamplerExportData, GfxSamplerProxy, GfxSamplerExporterRuntime>
	{
	ZENClassDeclare(GfxSampler, TResource)		
	};

	class GfxStateBlend : public TResource<GfxStateBlend, GfxStateBlendExportData, GfxStateBlendProxy, GfxStateBlendExporterRuntime>
	{
	ZENClassDeclare(GfxStateBlend, TResource)		
	};

	class GfxStateDepthStencil : public TResource<GfxStateDepthStencil, GfxStateDepthStencilExportData, GfxStateDepthStencilProxy, GfxStateDepthStencilExporterRuntime>
	{
	ZENClassDeclare(GfxStateDepthStencil, TResource)		
	};

	class GfxStateRasterizer : public TResource<GfxStateRasterizer, GfxStateRasterizerExportData, GfxStateRasterizerProxy, GfxStateRasterizerExporterRuntime>
	{
	ZENClassDeclare(GfxStateRasterizer, TResource)		
	};
	
}

#endif
