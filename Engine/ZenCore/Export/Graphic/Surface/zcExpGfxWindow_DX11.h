#pragma once
#if !defined(__zCore_Exp_Gfx_GfxWindow_DX11_h__) && ZEN_EXPORT_OR_RESOURCE_DX11
#define __zCore_Exp_Gfx_GfxWindow_DX11_h__

namespace zcExp
{
	class SerialGfxWindow_DX11 : public zcExp::ExportItem
	{
	ZENClassDeclare(SerialGfxWindow_DX11, zcExp::ExportItem)
	public:		
		struct ExportInfo : public zcExp::ExportInfoBase
		{				
			HWND					mhWindow;
		};
		virtual bool				ExportStart();
		
		HWND						mhWindow;
	};

	zResID CreateGfxWindow( HWND _WindowHandle );
}


#endif