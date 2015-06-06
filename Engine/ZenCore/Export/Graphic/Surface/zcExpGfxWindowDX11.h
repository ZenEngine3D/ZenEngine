#pragma once
#if !defined(__zCore_Exp_Gfx_GfxWindow_DX11_h__) && ZEN_EXPORT_OR_RESOURCE_DX11
#define __zCore_Exp_Gfx_GfxWindow_DX11_h__

namespace zcExp
{
	class ExportDataGfxWindowDX11 : public ExportDataBase
	{
	ZENClassDeclare(ExportDataGfxWindowDX11, ExportDataBase)
	public:
		virtual bool			Serialize( zcExp::Serializer_Base& _Serializer ){return true;}		
		HWND					mhWindow;
	};

}


#endif