#pragma once
#ifndef __zCore_Res_Gfx_GfxWindow_h__
#define __zCore_Res_Gfx_GfxWindow_h__

namespace zcRes
{
	class GfxWindow : public TResource<GfxWindow, GfxWindowResData, GfxWindowProxy, zcExp::ExporterNone>
	{
	ZENClassDeclare(GfxWindow, TResource)
	public:
		void						Resize(const zVec2U16& _vSize){mrProxy->mvPendingResize	 = _vSize; } //! @todo safety make this multithread safe
		void						SetBackbuffer(const GfxRenderTargetRef& _rBackbufferColor){mrBackbufferColor = _rBackbufferColor;}
		const GfxRenderTargetRef&	GetBackbuffer(){return mrBackbufferColor;}
	protected:
		GfxRenderTargetRef mrBackbufferColor;
	};

}

#endif