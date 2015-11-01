#pragma once
#ifndef __zCore_Res_Gfx_Renderpass_h__
#define __zCore_Res_Gfx_Renderpass_h__

namespace zcRes
{
	class GfxRenderPass : public TResource<GfxRenderPass, GfxRenderPassResData, GfxRenderPassProxy, GfxRenderPassExporterRuntime>
	{
	ZENClassDeclare(GfxRenderPass, TResource)
	};
}

#endif
