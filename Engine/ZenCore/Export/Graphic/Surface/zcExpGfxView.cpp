#include "zcCore.h"

namespace zcExp
{
	zResID ExportInfoGfxView::CallbackGetItemID(zenConst::eResPlatform _ePlatform, zenConst::eResType _eType, zenConst::eResSource _eSource, const zcExp::ExportInfoBase* _pExportInfo, bool& _bExistOut)
	{
		zenAssert(_eType==zenConst::keResType_GfxView);
		zenAssert( _pExportInfo );
		zResID::NameHash hName;
		const ExportInfoGfxView* pExportInfo = static_cast<const ExportInfoGfxView*>(_pExportInfo);
		hName.Append( (void*)pExportInfo->mpaRTColorConfig->First(), pExportInfo->mpaRTColorConfig->SizeMem() );
		hName.Append( (void*)&pExportInfo->mpRTDepthConfig, sizeof(pExportInfo->mpRTDepthConfig) );
		hName.Append( (void*)&pExportInfo->mvDim, sizeof(pExportInfo->mvDim) );
		hName.Append( (void*)&pExportInfo->mvOrigin, sizeof(pExportInfo->mvOrigin) );
		return zcExp::ValidateItemID(_ePlatform, _eType, _eSource, hName, _bExistOut);
	}

	ExporterGfxView::ExporterGfxView(const ExportResultRef& _rExportOut)
	: Super(_rExportOut.GetSafe())
	, mrExportResult(_rExportOut)
	{
	}

	bool ExporterGfxView::ExportStart()
	{
		ExportInfoGfxView* pExportInfo = static_cast<ExportInfoGfxView*>(mpExportInfo);
		if( !Super::ExportStart() )
			return false;		
		
		zenAssert(pExportInfo->mpaRTColorConfig);
		zenAssert(pExportInfo->mpRTDepthConfig);

		mrExportResult->mvDim				= pExportInfo->mvDim;		
		zenRes::zGfxTarget2D rParamDepthRT	= pExportInfo->mpRTDepthConfig->mrTargetSurface;
		if( rParamDepthRT.IsValid() )
		{
			const zenRes::zGfxRenderPass::ConfigDepthRT& RTConfig	= *pExportInfo->mpRTDepthConfig;
			zcRes::GfxTarget2DRef rDepthSurface						= RTConfig.mrTargetSurface;
			if( rDepthSurface.IsValid() )
			{
				mrExportResult->mvDim.x		= zenMath::Min<zU16>(mrExportResult->mvDim.x, rDepthSurface->GetDim().x-pExportInfo->mvOrigin.x );
				mrExportResult->mvDim.y		= zenMath::Min<zU16>(mrExportResult->mvDim.y, rDepthSurface->GetDim().y-pExportInfo->mvOrigin.y );
			}
		}

		for(zUInt rtIdx(0), rtCount(pExportInfo->mpaRTColorConfig->Count()); rtIdx<rtCount; ++rtIdx)
		{
			const zenRes::zGfxRenderPass::ConfigColorRT& RTConfig	= (*pExportInfo->mpaRTColorConfig)[rtIdx];
			zcRes::GfxTarget2DRef rColorSurface						= RTConfig.mrTargetSurface;
			//! @todo Missing: error if not found
			if( rColorSurface.IsValid() )
			{
				mrExportResult->mvDim.x		= zenMath::Min<zU16>(mrExportResult->mvDim.x, rColorSurface->GetDim().x-pExportInfo->mvOrigin.x );
				mrExportResult->mvDim.y		= zenMath::Min<zU16>(mrExportResult->mvDim.y, rColorSurface->GetDim().y-pExportInfo->mvOrigin.y );
			}			
		}

		mrExportResult->mvOrigin.x			= zenMath::Min<zU16>(pExportInfo->mvOrigin.x, mrExportResult->mvDim.x);
		mrExportResult->mvOrigin.y			= zenMath::Min<zU16>(pExportInfo->mvOrigin.y, mrExportResult->mvDim.y);
		mrExportResult->maRTColorConfig		= *pExportInfo->mpaRTColorConfig;
		mrExportResult->mRTDepthConfig		= *pExportInfo->mpRTDepthConfig;
		ExportSkipWork();
		return true;
	}

	//=================================================================================================
	//! @brief		Create a new View Resource
	//! @details	This bind together Color rendertarget(s), Depth rendertarget, and viewport infos
	//-------------------------------------------------------------------------------------------------
	//! @param _aRTColorConfig	- Array of RenderTargets to bind to each MRT point (optional)
	//! @param _RTDepthConfig	- Depth RenderTarget (optional)
	//! @param _vDim			- Viewport size (entire target size by default)
	//! @param _vOrigin			- Viewport origin ([0,0] by default)
	//! @return 				- Unique zResID of created Resource
	//=================================================================================================
	zResID CreateGfxView( const zArrayBase<zenRes::zGfxRenderPass::ConfigColorRT>& _aRTColorConfig, const zenRes::zGfxRenderPass::ConfigDepthRT& _RTDepthConfig, const zVec2U16& _vDim, const zVec2S16& _vOrigin )
	{
		static zenMem::zAllocatorPool sMemPool("Pool Views", sizeof(ExportInfoGfxView), 1, 5 );
		ExportInfoGfxView* pExportInfo	= zenNew(&sMemPool) ExportInfoGfxView;
		pExportInfo->mpaRTColorConfig		= &_aRTColorConfig;
		pExportInfo->mpRTDepthConfig		= &_RTDepthConfig;
		pExportInfo->mvDim					= _vDim;
		pExportInfo->mvOrigin				= _vOrigin;
		return zcMgr::Export.CreateItem( zResID::kePlatformType_GFX, zenConst::keResType_GfxView, pExportInfo );
	}

}

