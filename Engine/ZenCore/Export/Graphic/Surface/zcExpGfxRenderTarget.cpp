#include "zcCore.h"

namespace zcExp
{
	ExporterGfxRenderTarget::ExporterGfxRenderTarget(const ResDataRef& _rResData)
	: Super(_rResData.GetSafe())
	, mrResData(_rResData)
	{
	}

	bool ExporterGfxRenderTarget::ExportStart()
	{
		ExportSkipWork();
		if( Super::ExportStart() )
		{
			ExportInfoGfxRendertarget* pExportInfo	= static_cast<ExportInfoGfxRendertarget*>(mpExportInfo);		
			mrResData->meFormat						= pExportInfo->meFormat;
			mrResData->mvDim						= pExportInfo->mvDim;
			mrResData->mbSRGB						= pExportInfo->mbSRGB;
			return true;
		}
		return false;
	}

	//=================================================================================================
	//! @brief		Create a new RenderTarget Resource
	//! @details	
	//-------------------------------------------------------------------------------------------------
	//! @param _eFormat			- Format of raw data
	//! @param _vDim			- Width/Height of input image
	//! @param _bRgb			- If we're rendering to a srgb target	
	//! @return 				- Unique zResID of created Resource
	//=================================================================================================
	zResID CreateGfxRenderTarget(zenConst::eTextureFormat _eFormat, zVec2U16 _vDim, bool _bSrgb)
	{		
		static zenMem::zAllocatorPool sMemPool("Pool RenderTarget", sizeof(ExportInfoGfxRendertarget), 1, 5 );
		ExportInfoGfxRendertarget* pExportInfo	= zenNew(&sMemPool) ExportInfoGfxRendertarget;		
		pExportInfo->meFormat						= _eFormat;
		pExportInfo->mvDim							= _vDim;
		pExportInfo->mbSRGB							= _bSrgb;
		return zcMgr::Export.CreateItem( zResID::kePlatformType_GFX, zenConst::keResType_GfxRenderTarget, pExportInfo );
	}

}

