#include "zcCore.h"

namespace zcExp
{

ExporterGfxTarget2D::ExporterGfxTarget2D(const ExportResultRef& _rExportOut)
: Super(_rExportOut.GetSafe())
, mrExport(_rExportOut)
{
	zenAssert(mrExport.IsValid());
}

bool ExporterGfxTarget2D::ExportStart()
{
	ExportSkipWork();
	if( Super::ExportStart() )
	{
		ExportInfoGfxRendertarget* pExportInfo	= static_cast<ExportInfoGfxRendertarget*>(mpExportInfo);
		mrExport->mTextureID					= zcExp::CreateGfxTexture2D(pExportInfo->meFormat, zFlagResTexUse(keTexCreate_RenderTarget), pExportInfo->mvDim );
		mrExport->meFormat						= pExportInfo->meFormat;
		mrExport->mvDim							= pExportInfo->mvDim;
		mrExport->mbSRGB						= pExportInfo->mbSRGB;
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
//! @param _bRgb			- If we're rendering to a sRGB target	
//! @return 				- Unique zResID of created Resource
//=================================================================================================
zResID CreateGfxRenderTarget(zenConst::eTextureFormat _eFormat, zVec2U16 _vDim, bool _bSrgb)
{		
	static zenMem::zAllocatorPool sMemPool("Pool RenderTarget", sizeof(ExportInfoGfxRendertarget), 32, 32 );
	ExportInfoGfxRendertarget* pExportInfo	= zenNew(&sMemPool) ExportInfoGfxRendertarget;		
	pExportInfo->meFormat					= _eFormat;
	pExportInfo->mvDim						= _vDim;
	pExportInfo->mbSRGB						= _bSrgb;
	return zcMgr::Export.CreateItem( zResID::kePlatformType_GFX, zenConst::keResType_GfxTarget2D, pExportInfo );
}

}

