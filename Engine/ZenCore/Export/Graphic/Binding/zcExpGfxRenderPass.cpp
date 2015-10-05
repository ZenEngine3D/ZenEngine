#include "zcCore.h"

namespace zcExp
{
	ExporterGfxRenderPass::ExporterGfxRenderPass(const ResDataRef& _rResData)
	: Super(_rResData.GetSafe())
	, mrResData(_rResData)
	{
	}

	bool ExporterGfxRenderPass::ExportStart()
	{	
		if( !Super::ExportStart() )
			return false;

		ExportInfoGfxRenderPass* pExportInfo	= static_cast<ExportInfoGfxRenderPass*>(mpExportInfo);
		mrResData->mzStageName					= pExportInfo->mzStageName;
		mrResData->mBlendStateID				= pExportInfo->mBlendStateID;
		mrResData->mDepthStencilStateID			= pExportInfo->mDepthStencilStateID;
		mrResData->mRasterStateID				= pExportInfo->mRasterStateID;
		mrResData->mViewStateID					= pExportInfo->mViewStateID;
		ExportSkipWork();		
		return true;
	}

	//=================================================================================================
	//! @brief		Create a new RenderPass Resource
	//! @details	Tie together various GPU states setting for rendering (blending, depthtest...)
	//-------------------------------------------------------------------------------------------------
	//! @param 
	//! @param 
	//! @param 
	//! @param 
	//! @return 				- Unique zResID of created Resource
	//=================================================================================================
	zResID CreateGfxRenderPass(const zString& _zStageName, zResID _BlendStateID, zResID _DepthStencilStateID, zResID _RasterStateID, zResID _ViewStateID)
	{
		static zenMem::zAllocatorPool sMemPool("Pool RenderPass", sizeof(ExportInfoGfxRenderPass), 1, 5 );
		ExportInfoGfxRenderPass* pExportInfo	= zenNew(&sMemPool) ExportInfoGfxRenderPass;		
		pExportInfo->mzStageName				= _zStageName;
		pExportInfo->mBlendStateID				= _BlendStateID;
		pExportInfo->mDepthStencilStateID		= _DepthStencilStateID;
		pExportInfo->mRasterStateID				= _RasterStateID;
		pExportInfo->mViewStateID				= _ViewStateID;
		return zcMgr::Export.CreateItem( zResID::kePlatformType_GFX, zenConst::keResType_GfxRenderPass, pExportInfo );
	}

}

