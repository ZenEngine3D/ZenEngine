#pragma once
#ifndef __zCore_Res_Gfx_Renderpass_DX11_h__
#define __zCore_Res_Gfx_Renderpass_DX11_h__

namespace zcRes
{
	//=============================================================================================
	//! @class	Bind together all the GPU states needed for a drawcall
	//=============================================================================================
	class GfxRenderPassProxy_DX11 : public zRefCountedAutoDel
	{
	ZENClassDeclare(GfxRenderPassProxy_DX11, zRefCountedAutoDel)
	public:
												GfxRenderPassProxy_DX11();
		virtual									~GfxRenderPassProxy_DX11();
		bool									Initialize(class GfxRenderPass& _Owner);
		zString									mzStageName;
		GfxStateBlendProxyRef					mrProxBlendState;
		GfxStateDepthStencilProxyRef			mrProxDepthStencilState;
		GfxStateRasterizerProxyRef				mrProxRasterState;
		GfxViewProxyRef							mrProxViewState;
		ZENDbgCode(class GfxRenderPass*			mpOwner);
	};
}

#endif
