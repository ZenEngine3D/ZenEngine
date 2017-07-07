#pragma once

namespace zcExp
{
	class ExportGfxWindowDX12 : public zenRes::zExportData
	{
	zenClassDeclare(ExportGfxWindowDX12, zenRes::zExportData)
	public:
		HWND					mhWindow;
	};
}
