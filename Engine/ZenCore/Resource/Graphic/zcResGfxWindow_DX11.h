#pragma once
#ifndef __zCore_Res_Gfx_GfxWindow_DX11_h__
#define __zCore_Res_Gfx_GfxWindow_DX11_h__

namespace zcRes
{
	class GfxWindowProxy_DX11 : public zRefCountedAutoDel
	{
	ZENClassDeclare(GfxWindowProxy_DX11, zRefCountedAutoDel)
	public:
											GfxWindowProxy_DX11();
		virtual								~GfxWindowProxy_DX11();												
		bool								Initialize(class GfxWindow& _Owner);
		
		void								PerformResize();
		zVec2U16							mvPendingResize;

		ZENInline zcRes::GfxViewProxyRef	GetBackbuffer(){ return mrProxBackbufferView; }	//! @todo clean move to non proxy		

		IDXGISwapChain*						mDX11pSwapChain;
		GfxRenderTargetProxyRef				mrProxBackbufferColor;
		GfxRenderTargetProxyRef				mrProxBackbufferDepth;
		GfxViewProxyRef						mrProxBackbufferView;
		zenConst::eTextureFormat			meBackbufferColorFormat;
		zenConst::eTextureFormat			meBackbufferDepthFormat;
		zVec2U16							mvSize;
		ZENDbgCode(class GfxWindow*			mpOwner);
	};
}

#endif