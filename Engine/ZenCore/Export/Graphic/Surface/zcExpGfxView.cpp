#include "zcCore.h"

namespace zcExp
{
	zResID ExportInfoGfxView::CallbackGetItemID(zenConst::eResPlatform _ePlatform, zenConst::eResType _eType, zenConst::eResSource _eSource, const zcExp::ExportInfoBase* _pExportInfo, bool& _bExistOut)
	{
		ZENAssert(_eType==zenConst::keResType_GfxView);
		ZENAssert( _pExportInfo );
		const ExportInfoGfxView* pExportInfo = static_cast<const ExportInfoGfxView*>(_pExportInfo);

		zResID::NameHash hName;
		hName.Append( &pExportInfo->mvDim,		sizeof(&pExportInfo->mvDim) );
		hName.Append( &pExportInfo->mvOrigin,	sizeof(&pExportInfo->mvOrigin) );
		hName.Append( &pExportInfo->mTargetDepthID, sizeof(zResID) );
		for(zUInt rtIdx(0), rtCount(pExportInfo->maTargetColorID.Count()); rtIdx<rtCount; ++rtIdx)
			hName.Append( &(pExportInfo->maTargetColorID[rtIdx]), sizeof(zResID) );

		return zcExp::ValidateItemID(_ePlatform, _eType, _eSource, hName, _bExistOut);
	}

	ExporterGfxView::ExporterGfxView(const ResDataRef& _rResData)
	: Super(_rResData.GetSafe())
	, mrResData(_rResData)
	{
	}

	bool ExporterGfxView::ExportStart()
	{
		ExportInfoGfxView* pExportInfo = static_cast<ExportInfoGfxView*>(mpExportInfo);
		if( !Super::ExportStart() )
			return false;		

		zVec2U16 vDim											= pExportInfo->mvDim;
		zEngineConstRef<ResDataGfxRenderTarget> rParamRender	= zcDepot::ResourceData.GetItem<ResDataGfxRenderTarget>( pExportInfo->mTargetDepthID );
		if( rParamRender.IsValid() )	
		{
			vDim.x = zenMath::Min<zU16>(vDim.x, rParamRender->mvDim.x-pExportInfo->mvOrigin.x );
			vDim.y = zenMath::Min<zU16>(vDim.y, rParamRender->mvDim.y-pExportInfo->mvOrigin.y );
		}

		for(zUInt rtIdx(0), rtCount(pExportInfo->maTargetColorID.Count()); rtIdx<rtCount; ++rtIdx)
		{
			rParamRender = zcDepot::ResourceData.GetItem<ResDataGfxRenderTarget>( pExportInfo->maTargetColorID[rtIdx] );
			//! @todo Missing: error if mismatch size
			if( rParamRender.IsValid() )
			{
				vDim.x = zenMath::Min<zU16>(vDim.x, rParamRender->mvDim.x-pExportInfo->mvOrigin.x );
				vDim.y = zenMath::Min<zU16>(vDim.y, rParamRender->mvDim.y-pExportInfo->mvOrigin.y );
			}			
		}
		
		mrResData->maTargetColorID	= pExportInfo->maTargetColorID;
		mrResData->mTargetDepthID	= pExportInfo->mTargetDepthID;
		mrResData->mvDim				= vDim;
		mrResData->mvOrigin			= pExportInfo->mvOrigin;
		ExportSkipWork();
		return true;
	}

	//=================================================================================================
	//! @brief		Create a new View Resource
	//! @details	This bind together Color rendertarget(s), Depth rendertarget, and viewport infos
	//-------------------------------------------------------------------------------------------------
	//! @param _TargetColorID	- Color RenderTarget (empty zResID() if none)
	//! @param _TargetDepthID	- Depth RenderTarget (empty zResID() if none)
	//! @param _vSize			- Viewport size (entire target size by default)
	//! @param _vOrigin			- Viewport origin ([0,0] by default)
	//! @return 				- Unique zResID of created Resource
	//=================================================================================================
	zResID CreateGfxView( const zResID& _TargetColorID, const zResID& _TargetDepthID, const zVec2U16& _vDim, const zVec2S16& _vOrigin )
	{
		static zenMem::zAllocatorPool sMemPool("Pool Views", sizeof(ExportInfoGfxView), 1, 5 );
		ExportInfoGfxView* pExportInfo	= zenNew(&sMemPool) ExportInfoGfxView;
		pExportInfo->maTargetColorID.Copy(&_TargetColorID, 1);		
		pExportInfo->mTargetDepthID			= _TargetDepthID;
		pExportInfo->mvOrigin				= _vOrigin;
		pExportInfo->mvDim					= _vDim;
		return zcMgr::Export.CreateItem( zResID::kePlatformType_GFX, zenConst::keResType_GfxView, pExportInfo );
	}

	//=================================================================================================
	//! @brief		Create a new View Resource
	//! @details	This bind together Color rendertarget(s), Depth rendertarget, and viewport infos
	//-------------------------------------------------------------------------------------------------
	//! @param _aTargetColorID	- Array of RenderTargets to bind to each MRT point (optional)
	//! @param _TargetDepthID	- Depth RenderTarget (optional)
	//! @param _vSize			- Viewport size (entire target size by default)
	//! @param _vOrigin			- Viewport origin ([0,0] by default)
	//! @return 				- Unique zResID of created Resource
	//=================================================================================================
	zResID CreateGfxView( const zArrayBase<zResID>& _aTargetColorID, const zResID& _TargetDepthID, const zVec2U16& _vDim, const zVec2S16& _vOrigin )
	{
		static zenMem::zAllocatorPool sMemPool("Pool Views", sizeof(ExportInfoGfxView), 1, 5 );
		ExportInfoGfxView* pExportInfo	= zenNew(&sMemPool) ExportInfoGfxView;
		pExportInfo->maTargetColorID		= _aTargetColorID;
		pExportInfo->mTargetDepthID			= _TargetDepthID;
		pExportInfo->mvOrigin				= _vOrigin;
		pExportInfo->mvDim					= _vDim;
		return zcMgr::Export.CreateItem( zResID::kePlatformType_GFX, zenConst::keResType_GfxView, pExportInfo );
	}

}

