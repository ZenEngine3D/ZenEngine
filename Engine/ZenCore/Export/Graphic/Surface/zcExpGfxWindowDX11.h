#pragma once

namespace zcExp
{
	class ExportGfxWindowDX11 : public zenRes::zExportData
	{
	zenClassDeclare(ExportGfxWindowDX11, zenRes::zExportData)
	public:
		HWND					mhWindow;
	};
}
