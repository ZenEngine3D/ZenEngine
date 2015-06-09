#include "zcCore.h"
#if ZEN_EXPORT_OR_RESOURCE_DX11

namespace zcExp
{
/*
	//=================================================================================================
	//! @brief		Copy the wanted window handle
	//! @details	Since there's no work proper here, cancel the work phase of export
	//-------------------------------------------------------------------------------------------------
	//! @return 	- True if successful
	//=================================================================================================
	bool SerialGfxWindow_DX11::ExportStart()
	{
		ExportInfo* pExportInfo = static_cast<ExportInfo*>(mpExportInfo);
		if( !Super::ExportStart() )
			return false;
		
		mhWindow = pExportInfo->mhWindow;
		ExportSkipWork();
		return true;
	}

	//=================================================================================================
	//! @brief		Create a render window resource
	//! @details	Each platform need to define this, and the code is platform specific with no
	//!				common export class.
	//!				This will result in DirectX creating a render swapchain with associated Window.
	//!	@Note		Because Windows has a define named CreateWindow, we can't use it for our function's
	//				name. Ending it with 'Gfx' to make it different
	//! @todo Clean: Make it cross platform (windows handle)
	//-------------------------------------------------------------------------------------------------
	//! @param _WindowHandle	- Parent Window
	//! @return 				- Unique zResID of created Resource
	//=================================================================================================
	zResID CreateGfxWindow( HWND _WindowHandle )
	{
		static zenMem::zAllocatorPool sMemPool("Pool Views", sizeof(SerialGfxWindow_DX11::ExportInfo), 1, 5 );
		SerialGfxWindow_DX11::ExportInfo* pExportInfo	= zenNew(&sMemPool) SerialGfxWindow_DX11::ExportInfo;
		pExportInfo->mhWindow							= _WindowHandle;
		return EMgr::Export.CreateItem( zResID::kePlatformType_GFX, zenConst::keResType_GfxWindow, pExportInfo );
	}
	*/

}

#endif //ZEN_EXPORT_OR_RESOURCE_DX11