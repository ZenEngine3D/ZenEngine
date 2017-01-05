#pragma once
#ifndef __zCore_Exp_Gfx_GfxWindow_DX11_h__
#define __zCore_Exp_Gfx_GfxWindow_DX11_h__

namespace zcExp
{
	class ExportGfxWindowDX11 : public zenRes::zExportData
	{
	zenClassDeclare(ExportGfxWindowDX11, zenRes::zExportData)
	public:
		HWND					mhWindow;
	};
}

#endif
