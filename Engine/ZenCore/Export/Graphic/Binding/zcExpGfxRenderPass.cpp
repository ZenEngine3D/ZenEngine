#include "zcCore.h"

namespace zcExp
{
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
	zResID CreateGfxRenderpass(const zString& _zStageName, zUInt _uPassPriority, const zenRes::zGfxRenderPass::ConfigColorRT& _ConfigRTColor, const zenRes::zGfxRenderPass::ConfigDepthRT& _ConfigRTDepth, zenRes::zGfxStateRasterizer _rStateRaster, const zVec2U16& _vDim, const zVec2S16& _vOrigin)
	{
		zArrayStatic<zenRes::zGfxRenderPass::ConfigColorRT> aColorRTConfig = {_ConfigRTColor};
		return CreateGfxRenderpass(_zStageName, _uPassPriority, aColorRTConfig, _ConfigRTDepth, _rStateRaster, _vDim, _vOrigin);
	}
	
	zResID CreateGfxRenderpass(const zString& _zStageName, zUInt _uPassPriority, const zArrayBase<zenRes::zGfxRenderPass::ConfigColorRT>& _aConfigRTColor, const zenRes::zGfxRenderPass::ConfigDepthRT& _ConfigRTDepth, zenRes::zGfxStateRasterizer _rStateRaster, const zVec2U16& _vDim, const zVec2S16& _vOrigin)
	{
		static zenMem::zAllocatorPool sMemPool("Pool RenderPass", sizeof(ExportInfoGfxRenderpass), 1, 5 );
		ExportInfoGfxRenderpass* pExportInfo	= zenNew(&sMemPool) ExportInfoGfxRenderpass;		
		pExportInfo->mzStageName				= _zStageName;
		pExportInfo->muPassPriority				= _uPassPriority;		
		pExportInfo->mpConfigRTDepth			= &_ConfigRTDepth;		
		pExportInfo->mpaConfigRTColor			= &_aConfigRTColor;
		pExportInfo->mrStateRaster				= _rStateRaster;
		pExportInfo->mvDim						= _vDim;
		pExportInfo->mvOrigin					= _vOrigin;
		return zcMgr::Export.CreateItem( zResID::kePlatformType_GFX, zenConst::keResType_GfxRenderPass, pExportInfo );
	}


}

