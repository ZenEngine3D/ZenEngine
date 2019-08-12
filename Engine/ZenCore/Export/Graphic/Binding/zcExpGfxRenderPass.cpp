#include "zcCore.h"

namespace zcExp
{
	ExporterGfxRenderPass::ExporterGfxRenderPass(const ExportResultRef& _rExportOut)
	: Super(_rExportOut.GetSafe())
	, mrExport(_rExportOut)
	{
		zenAssert( _rExportOut.IsValid() );
	}

	bool ExporterGfxRenderPass::ExportStart()
	{	
		if( !Super::ExportStart() )
			return false;

		ExportInfoGfxRenderpass* pExportInfo	= static_cast<ExportInfoGfxRenderpass*>(mpExportInfo);
		mrExport->mzStageName					= pExportInfo->mzStageName;
		mrExport->mRasterStateID				= pExportInfo->mRasterStateID;
		mrExport->muPassPriority				= pExportInfo->muPassPriority;
		mrExport->mRasterStateID				= pExportInfo->mrStateRaster.GetResID(); //! @todo missing : Create a RasterState per culling config
		mrExport->mViewStateID					= CreateGfxView(*pExportInfo->mpaConfigRTColor, *pExportInfo->mpConfigRTDepth, pExportInfo->mvDim, pExportInfo->mvOrigin);		
		mrExport->mBlendStateID					= CreateGfxBlend(*pExportInfo->mpaConfigRTColor); 
		mrExport->mDepthStencilStateID			= CreateGfxDepthStencil(*pExportInfo->mpConfigRTDepth);			
		ExportSkipWork();		
		return true;
	}

	//=================================================================================================
	//! @brief		Create a new RenderPass Resource
	//! @details	Tie together various GPU states setting for rendering (blending, depthtest...)
	//-------------------------------------------------------------------------------------------------
	//! @param _zStageName		- Debug name displayed for this render stage (in GPU/CPU events)
	//! @param _uPassPriority	- Renderpass priority over other Renderpass when dispatched in together. Lower number executed first
	//! @param _ConfigRTColor	- Color Rendertarget(s) configuration
	//! @param _ConfigRTDepth	- Depth Rendertarget configuration
	//! @param _rStateRaster	- GPU Rasterizer configuration (backface culling, wireframe, ...)
	//! @return 				- Unique zResID of created Resource
	//=================================================================================================
	zResID CreateGfxRenderpass(const zString& _zStageName, zUInt _uPassPriority, const zenRes::zGfxRenderPass::ConfigColorRT& _ConfigRTColor, const zenRes::zGfxRenderPass::ConfigDepthRT& _ConfigRTDepth, zenRes::zGfxStateRaster _rStateRaster, const zVec2U16& _vDim, const zVec2S16& _vOrigin)
	{
		zArrayFixed<zenRes::zGfxRenderPass::ConfigColorRT, 1> aColorRTConfig = {_ConfigRTColor};
		return CreateGfxRenderpass(_zStageName, _uPassPriority, aColorRTConfig, _ConfigRTDepth, _rStateRaster, _vDim, _vOrigin);
	}
	
	zResID CreateGfxRenderpass(const zString& _zStageName, zUInt _uPassPriority, const zArray<zenRes::zGfxRenderPass::ConfigColorRT>& _aConfigRTColor, const zenRes::zGfxRenderPass::ConfigDepthRT& _ConfigRTDepth, zenRes::zGfxStateRaster _rStateRaster, const zVec2U16& _vDim, const zVec2S16& _vOrigin)
	{
		auto* pExportInfo				= zenMem::NewPool<ExportInfoGfxRenderpass>();
		pExportInfo->mzStageName		= _zStageName;
		pExportInfo->muPassPriority		= _uPassPriority;		
		pExportInfo->mpConfigRTDepth	= &_ConfigRTDepth;		
		pExportInfo->mpaConfigRTColor	= &_aConfigRTColor;
		pExportInfo->mrStateRaster		= _rStateRaster;
		pExportInfo->mvDim				= _vDim;
		pExportInfo->mvOrigin			= _vOrigin;
		return zcMgr::Export.CreateItem( zResID::kePlatformType_GFX, zenConst::keResType_GfxRenderPass, pExportInfo );
	}


}

