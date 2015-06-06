#pragma once
#ifndef __zCore_Res_Gfx_State_h__
#define __zCore_Res_Gfx_State_h__

namespace zcRes
{
	class GfxSampler : public TResource<GfxSampler, GfxSamplerResData, GfxSamplerProxy, GfxSamplerExporterRuntime>
	{
	ZENClassDeclare(GfxSampler, TResource)		
	};

	class GfxStateBlend : public TResource<GfxStateBlend, GfxStateBlendResData, GfxStateBlendProxy, GfxStateBlendExporterRuntime>
	{
	ZENClassDeclare(GfxStateBlend, TResource)		
	};

	class GfxStateDepthStencil : public TResource<GfxStateDepthStencil, GfxStateDepthStencilResData, GfxStateDepthStencilProxy, GfxStateDepthStencilExporterRuntime>
	{
	ZENClassDeclare(GfxStateDepthStencil, TResource)		
	};

	class GfxStateRasterizer : public TResource<GfxStateRasterizer, GfxStateRasterizerResData, GfxStateRasterizerProxy, GfxStateRasterizerExporterRuntime>
	{
	ZENClassDeclare(GfxStateRasterizer, TResource)		
	};
	
}

#endif
