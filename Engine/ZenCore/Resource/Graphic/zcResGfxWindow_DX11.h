#pragma once
#ifndef __zCore_Res_Gfx_GfxWindow_DX11_h__
#define __zCore_Res_Gfx_GfxWindow_DX11_h__

namespace zcRes
{
	class GfxWindowProxy_DX11 : public zRefCountedAutoDel
	{
	ZENClassDeclare(GfxWindowProxy_DX11, zRefCountedAutoDel)
	public:
		virtual								~GfxWindowProxy_DX11();												
		bool								Initialize(class GfxWindow& _Owner);		
		void								PerformResize();
		
		IDXGISwapChain*						mDX11pSwapChain			= nullptr;
		GfxRenderTargetProxyRef				mrProxBackbufferColor	= nullptr;
		zenConst::eTextureFormat			meBackbufferColorFormat	= zenConst::keTexFormat_RGBA8; //! @todo clean feature expose desired format in ResData
		zVec2U16							mvSize					= zVec2U16(0,0);
		zVec2U16							mvPendingResize			= zVec2U16(0,0);
		ZENDbgCode(class GfxWindow*			mpOwner);
	};
}

#endif