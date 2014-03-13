#include "libZenCore.h"

namespace EExp
{
	//=================================================================================================
	//! @brief		Associate a serie of RenderTarget together
	//! @details	Since there's no work proper here, cancel the work phase of export
	//-------------------------------------------------------------------------------------------------
	//! @return 	- True if successful
	//=================================================================================================
	bool SerialGfxView_Base::ExportStart()
	{
		ExportInfo* pExportInfo = static_cast<ExportInfo*>(mpExportInfo);
		if( !Super::ExportStart() )
			return false;		

		zVec2U16 vDim	= pExportInfo->mvDim;
		const SerialRenderTarget_Base* pParamRender = EMgr::SerialItems.GetItem<const SerialRenderTarget_Base>( pExportInfo->mTargetDepthID );
		if( pParamRender )	
		{
			vDim.x = zenMath::Min<zU16>(vDim.x, pParamRender->mvDim.x-pExportInfo->mvOrigin.x );
			vDim.y = zenMath::Min<zU16>(vDim.y, pParamRender->mvDim.y-pExportInfo->mvOrigin.y );
		}

		for(zUInt rtIdx(0), rtCount(pExportInfo->maTargetColorID.Count()); rtIdx<rtCount; ++rtIdx)
		{
			pParamRender = EMgr::SerialItems.GetItem<const SerialRenderTarget_Base>( pExportInfo->maTargetColorID[rtIdx] );
			//! @todo error if mismatch size
			if( pParamRender )
			{
				vDim.x = zenMath::Min<zU16>(vDim.x, pParamRender->mvDim.x-pExportInfo->mvOrigin.x );
				vDim.y = zenMath::Min<zU16>(vDim.y, pParamRender->mvDim.y-pExportInfo->mvOrigin.y );
			}			
		}
		
		mSerial.maTargetColorID = pExportInfo->maTargetColorID;
		mSerial.mTargetDepthID	= pExportInfo->mTargetDepthID;
		mvDim					= vDim;
		mvOrigin				= pExportInfo->mvOrigin;
		ExportSkipWork();
		return true;
	}

	zResID SerialGfxView_Base::CallbackGetItemID(zenConst::eResPlatform _ePlatform, zenConst::eResType _eType, zenConst::eResSource _eSource, const EExp::ExportInfoBase* _pExportInfo, bool& _bExistOut)
	{
		ZENAssert(_eType==zenConst::keResType_GfxView);
		ZENAssert( _pExportInfo );
		const ExportInfo* pExportInfo = static_cast<const ExportInfo*>(_pExportInfo);

		zResID::NameHash hName;
		hName.Append( &pExportInfo->mvDim,		sizeof(&pExportInfo->mvDim) );
		hName.Append( &pExportInfo->mvOrigin,	sizeof(&pExportInfo->mvOrigin) );
		hName.Append( &pExportInfo->mTargetDepthID, sizeof(zResID) );
		for(zUInt rtIdx(0), rtCount(pExportInfo->maTargetColorID.Count()); rtIdx<rtCount; ++rtIdx)
			hName.Append( &(pExportInfo->maTargetColorID[rtIdx]), sizeof(zResID) );

		return EExp::ValidateItemID(_ePlatform, _eType, _eSource, hName, _bExistOut);
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
	zResID CreateGfxView( const zResID& _TargetColorID, const zResID& _TargetDepthID, const zVec2U16& _vDim, const zVec2U16& _vOrigin )
	{
		static zenMem::zAllocatorPool sMemPool("Pool Views", sizeof(SerialGfxView_Base::ExportInfo), 1, 5 );
		SerialGfxView_Base::ExportInfo* pExportInfo	= zenNew(&sMemPool) SerialGfxView_Base::ExportInfo;
		pExportInfo->maTargetColorID.Copy(&_TargetColorID, 1);		
		pExportInfo->mTargetDepthID					= _TargetDepthID;
		pExportInfo->mvOrigin						= _vOrigin;
		pExportInfo->mvDim							= _vDim;
		return EMgr::Export.CreateItem( zResID::kePlatformType_GFX, zenConst::keResType_GfxView, pExportInfo );
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
	zResID CreateGfxView( const zArrayBase<zResID>& _aTargetColorID, const zResID& _TargetDepthID, const zVec2U16& _vDim, const zVec2U16& _vOrigin )
	{
		static zenMem::zAllocatorPool sMemPool("Pool Views", sizeof(SerialGfxView_Base::ExportInfo), 1, 5 );
		SerialGfxView_Base::ExportInfo* pExportInfo	= zenNew(&sMemPool) SerialGfxView_Base::ExportInfo;
		pExportInfo->maTargetColorID				= _aTargetColorID;
		pExportInfo->mTargetDepthID					= _TargetDepthID;
		pExportInfo->mvOrigin						= _vOrigin;
		pExportInfo->mvDim							= _vDim;
		return EMgr::Export.CreateItem( zResID::kePlatformType_GFX, zenConst::keResType_GfxView, pExportInfo );
	}

}

