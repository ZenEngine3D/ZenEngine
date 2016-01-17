#pragma once
#ifndef __zCore_Res_Gfx_GfxWindow_h__
#define __zCore_Res_Gfx_GfxWindow_h__

namespace zxImGui {	class zxRenderData; } //Forward declare
namespace zen { namespace zenWnd { class Window; } }

namespace zcRes
{
	class GfxWindow : public TResource<GfxWindow, GfxWindowResData, GfxWindowProxy, zcExp::ExporterNone>
	{
	ZENClassDeclare(GfxWindow, TResource)
	public:
											GfxWindow();
		void								Resize(const zVec2U16& _vSize){ mrProxy->mvPendingResize = _vSize; } //! @todo safety make this multithread safe
		void								SetBackbuffer(zU8 _uBackbufferIndex, const GfxRenderTargetRef& _rBackbufferColor);
		const GfxRenderTargetRef&			GetBackbuffer();
		void								FrameBegin();
		void								FrameEnd();
		zenSig::zSignalEmitter0&			GetSignalUIRender();
		zenWnd::Window*						mpMainWindowOS = nullptr; //! @todo urgent : temp hack until merged gfx + OS window
	protected:
		GfxRenderTargetRef					mrBackbufferColor[2];
		zEngineRef<zxImGui::zxRenderData>	mrImGuiData;
	};

}

#endif
