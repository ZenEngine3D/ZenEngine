#pragma once
#if !defined(__zCore_Exp_Gfx_GfxWindow_DX12_h__) && ZEN_EXPORT_OR_RESOURCE_DX12
#define __zCore_Exp_Gfx_GfxWindow_DX12_h__
//SF DX12

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