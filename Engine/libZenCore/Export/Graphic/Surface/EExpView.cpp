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

		zenVec2U16 vDim	= pExportInfo->mvDim;
		const SerialRenderTarget_Base* pParamRender = EMgr::SerialItems.GetItem<const SerialRenderTarget_Base>( pExportInfo->mTargetDepthID );
		if( pParamRender )	
		{
			vDim.x = zenMath::Min<zenU16>(vDim.x, pParamRender->mvDim.x-pExportInfo->mvOrigin.x );
			vDim.y = zenMath::Min<zenU16>(vDim.y, pParamRender->mvDim.y-pExportInfo->mvOrigin.y );
		}

		for(zenUInt rtIdx(0), rtCount(pExportInfo->maTargetColorID.Count()); rtIdx<rtCount; ++rtIdx)
		{
			pParamRender = EMgr::SerialItems.GetItem<const SerialRenderTarget_Base>( pExportInfo->maTargetColorID[rtIdx] );
			//! @todo error if mismatch size
			if( pParamRender )
			{
				vDim.x = zenMath::Min<zenU16>(vDim.x, pParamRender->mvDim.x-pExportInfo->mvOrigin.x );
				vDim.y = zenMath::Min<zenU16>(vDim.y, pParamRender->mvDim.y-pExportInfo->mvOrigin.y );
			}			
		}
		
		mSerial.maTargetColorID = pExportInfo->maTargetColorID;
		mSerial.mTargetDepthID	= pExportInfo->mTargetDepthID;
		mvDim					= vDim;
		mvOrigin				= pExportInfo->mvOrigin;
		ExportSkipWork();
		return true;
	}

	zenResID SerialGfxView_Base::CallbackGetItemID(awconst::eResPlatform _ePlatform, awconst::eResType _eType, awconst::eResSource _eSource, const EExp::ExportInfoBase* _pExportInfo, bool& _bExistOut)
	{
		ZENAssert(_eType==awconst::keResType_GfxView);
		ZENAssert( _pExportInfo );
		const ExportInfo* pExportInfo = static_cast<const ExportInfo*>(_pExportInfo);

		zenResID::NameHash hName;
		hName.Append( &pExportInfo->mvDim,		sizeof(&pExportInfo->mvDim) );
		hName.Append( &pExportInfo->mvOrigin,	sizeof(&pExportInfo->mvOrigin) );
		hName.Append( &pExportInfo->mTargetDepthID, sizeof(zenResID) );
		for(zenUInt rtIdx(0), rtCount(pExportInfo->maTargetColorID.Count()); rtIdx<rtCount; ++rtIdx)
			hName.Append( &(pExportInfo->maTargetColorID[rtIdx]), sizeof(zenResID) );

		return EExp::ValidateItemID(_ePlatform, _eType, _eSource, hName, _bExistOut);
	}

	//=================================================================================================
	//! @brief		Create a new View Resource
	//! @details	This bind together Color rendertarget(s), Depth rendertarget, and viewport infos
	//-------------------------------------------------------------------------------------------------
	//! @param _TargetColorID	- Color RenderTarget (empty zenResID() if none)
	//! @param _TargetDepthID	- Depth RenderTarget (empty zenResID() if none)
	//! @param _vSize			- Viewport size (entire target size by default)
	//! @param _vOrigin			- Viewport origin ([0,0] by default)
	//! @return 				- Unique zenResID of created Resource
	//=================================================================================================
	zenResID CreateGfxView( const zenResID& _TargetColorID, const zenResID& _TargetDepthID, const zenVec2U16& _vDim, const zenVec2U16& _vOrigin )
	{
		static zenMem::AllocatorPool sMemPool("Pool Views", sizeof(SerialGfxView_Base::ExportInfo), 1, 5 );
		SerialGfxView_Base::ExportInfo* pExportInfo	= zenNew(&sMemPool) SerialGfxView_Base::ExportInfo;
		pExportInfo->maTargetColorID.Copy(&_TargetColorID, 1);		
		pExportInfo->mTargetDepthID					= _TargetDepthID;
		pExportInfo->mvOrigin						= _vOrigin;
		pExportInfo->mvDim							= _vDim;
		return EMgr::Export.CreateItem( zenResID::kePlatformType_GFX, awconst::keResType_GfxView, pExportInfo );
	}

	//=================================================================================================
	//! @brief		Create a new View Resource
	//! @details	This bind together Color rendertarget(s), Depth rendertarget, and viewport infos
	//-------------------------------------------------------------------------------------------------
	//! @param _aTargetColorID	- Array of RenderTargets to bind to each MRT point (optional)
	//! @param _TargetDepthID	- Depth RenderTarget (optional)
	//! @param _vSize			- Viewport size (entire target size by default)
	//! @param _vOrigin			- Viewport origin ([0,0] by default)
	//! @return 				- Unique zenResID of created Resource
	//=================================================================================================
	zenResID CreateGfxView( const zenArrayBase<zenResID>& _aTargetColorID, const zenResID& _TargetDepthID, const zenVec2U16& _vDim, const zenVec2U16& _vOrigin )
	{
		static zenMem::AllocatorPool sMemPool("Pool Views", sizeof(SerialGfxView_Base::ExportInfo), 1, 5 );
		SerialGfxView_Base::ExportInfo* pExportInfo	= zenNew(&sMemPool) SerialGfxView_Base::ExportInfo;
		pExportInfo->maTargetColorID				= _aTargetColorID;
		pExportInfo->mTargetDepthID					= _TargetDepthID;
		pExportInfo->mvOrigin						= _vOrigin;
		pExportInfo->mvDim							= _vDim;
		return EMgr::Export.CreateItem( zenResID::kePlatformType_GFX, awconst::keResType_GfxView, pExportInfo );
	}

}

