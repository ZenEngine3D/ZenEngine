#pragma once
#ifndef __zCore_Res_Gfx_Renderpass_DX11_h__
#define __zCore_Res_Gfx_Renderpass_DX11_h__

namespace zcRes
{
	//=============================================================================================
	//! @class	Bind together all the GPU states needed for a drawcall
	//=============================================================================================
	class GfxRenderPassProxy_DX11 : public zRefCounted
	{
	ZENClassDeclare(GfxRenderPassProxy_DX11, zRefCounted)
	public:
											GfxRenderPassProxy_DX11();
		virtual								~GfxRenderPassProxy_DX11();
		bool								Initialize(class GfxRenderPass& _Owner);
		zString								mzStageName;
		GfxStateBlendRef					mrProxBlendState;
		GfxStateDepthStencilRef				mrProxDepthStencilState;
		GfxStateRasterizerRef				mrProxRasterState;
		GfxViewRef							mrProxViewState;
		ZENDbgCode(class GfxRenderPass*		mpOwner);
	};
}

#endif
