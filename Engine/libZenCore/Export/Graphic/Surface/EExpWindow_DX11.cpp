#include "libZenCore.h"
#if AW_EXPORT_OR_RESOURCE_DX11

namespace EExp
{
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
	//! @todo		Make it cross platform
	//-------------------------------------------------------------------------------------------------
	//! @param _WindowHandle	- Parent Window
	//! @return 				- Unique awResourceID of created Resource
	//=================================================================================================
	awResourceID CreateGfxWindow( HWND _WindowHandle )
	{
		static CMem::PoolAllocator sMemPool("Pool Views", sizeof(SerialGfxWindow_DX11::ExportInfo), 1, 5 );
		SerialGfxWindow_DX11::ExportInfo* pExportInfo	= AWNew(&sMemPool) SerialGfxWindow_DX11::ExportInfo;
		pExportInfo->mhWindow							= _WindowHandle;
		return EMgr::Export.CreateItem( awResourceID::kePlatformType_GFX, awconst::keResType_GfxWindow, pExportInfo );
	}

}

#endif //AW_EXPORT_OR_RESOURCE_DX11
