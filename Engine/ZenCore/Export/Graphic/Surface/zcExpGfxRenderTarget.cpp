#include "zcCore.h"

namespace zcExp
{
	ExporterGfxRenderTarget::ExporterGfxRenderTarget(const ExportDataRef& _rExportData)
	: Super(_rExportData.GetSafe())
	, mrExportData(_rExportData)
	{
	}

	bool ExporterGfxRenderTarget::ExportStart()
	{
		ExportSkipWork();
		if( Super::ExportStart() )
		{
			ExportInfoGfxRendertarget* pExportInfo	= static_cast<ExportInfoGfxRendertarget*>(mpExportInfo);		
			mrExportData->meFormat						= pExportInfo->meFormat;
			mrExportData->mvDim							= pExportInfo->mvDim;
			mrExportData->mbSRGB						= pExportInfo->mbSRGB;
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
		return EMgr::Export.CreateItem( zResID::kePlatformType_GFX, zenConst::keResType_GfxRenderTarget, pExportInfo );
	}

}

