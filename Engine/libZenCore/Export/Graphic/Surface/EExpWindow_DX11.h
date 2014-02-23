#pragma once
#if !defined(__LibExport_Graphics_GfxWindow_DX11_h__) && AW_EXPORT_OR_RESOURCE_DX11
#define __LibExport_Graphics_GfxWindow_DX11_h__

namespace EExp
{
	class SerialGfxWindow_DX11 : public EExp::ExportItem
	{
	AWClassDeclare(SerialGfxWindow_DX11, EExp::ExportItem)
	public:		
		struct ExportInfo : public EExp::ExportInfoBase
		{				
			HWND					mhWindow;
		};
		virtual bool				ExportStart();
		
		HWND						mhWindow;
	};

	zenResID CreateGfxWindow( HWND _WindowHandle );
}


#endif