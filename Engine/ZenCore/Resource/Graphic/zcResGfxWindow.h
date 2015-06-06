#pragma once
#ifndef __zCore_Res_Gfx_GfxWindow_h__
#define __zCore_Res_Gfx_GfxWindow_h__

namespace zcRes
{
	class GfxWindow : public TResource<GfxWindow, GfxWindowExportData, GfxWindowProxy, zcExp::ExporterNone>
	{
	ZENClassDeclare(GfxWindow, TResource)
	public:
		void				Resize(const zVec2U16& _vSize){mrProxy->mvPendingResize	 = _vSize; } //! @todo safety make this multithread safe
		void				SetBackbuffer(const GfxViewRef& _rView){mrView = _rView;}
		const GfxViewRef&	GetBackbuffer(){return mrView;}
	protected:
		GfxViewRef			mrView;			
	};

}

#endif