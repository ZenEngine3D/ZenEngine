#pragma once
#if !defined(__zCore_Exp_Gfx_GfxWindow_DX11_h__) && ZEN_EXPORT_OR_RESOURCE_DX11
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