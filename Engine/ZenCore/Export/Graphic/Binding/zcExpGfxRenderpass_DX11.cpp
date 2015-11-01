#include "zcCore.h"

namespace zcExp
{
	ExporterGfxRenderPass_DX11::ExporterGfxRenderPass_DX11(const ResDataRef& _rResData)
	: Super(_rResData.GetSafe())
	, mrResData(_rResData)
	{
	}

	bool ExporterGfxRenderPass_DX11::ExportStart()
	{	
		if( !Super::ExportStart() )
			return false;

		ExportInfoGfxRenderpass* pExportInfo	= static_cast<ExportInfoGfxRenderpass*>(mpExportInfo);
		mrResData->mzStageName					= pExportInfo->mzStageName;
		mrResData->mRasterStateID				= pExportInfo->mRasterStateID;
		mrResData->muPassPriority				= pExportInfo->muPassPriority;
		mrResData->mRasterStateID				= pExportInfo->mrStateRaster.GetResID();
		mrResData->mViewStateID					= CreateGfxView(*pExportInfo->mpaConfigRTColor, *pExportInfo->mpConfigRTDepth, pExportInfo->mvDim, pExportInfo->mvOrigin);		
		mrResData->mBlendStateID				= CreateGfxBlend(*pExportInfo->mpaConfigRTColor); 
		mrResData->mDepthStencilStateID			= CreateGfxDepthStencil(*pExportInfo->mpConfigRTDepth);			

		ExportSkipWork();		
		return true;
	}

}

