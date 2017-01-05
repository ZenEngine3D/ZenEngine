#pragma once
#ifndef __zCore_Exp_Gfx_GfxWindow_DX12_h__
#define __zCore_Exp_Gfx_GfxWindow_DX12_h__

namespace zcExp
{
	class ExportGfxWindowDX12 : public zenRes::zExportData
	{
	zenClassDeclare(ExportGfxWindowDX12, zenRes::zExportData)
	public:
		HWND					mhWindow;
	};
}

#endif