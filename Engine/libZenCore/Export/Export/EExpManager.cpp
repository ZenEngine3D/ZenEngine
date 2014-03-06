#include "libZenCore.h"

namespace EMgr { EExp::ManagerExport Export; }

namespace EExp
{

zenU32	ManagerExport::saNextID[zenConst::keResType__Count];

//=================================================================================================
//! @brief		Constructor
//=================================================================================================
ManagerExport::ManagerExport()
: muExportPending(0)
, maExportSuccess(256)
, maExportFail(256)
{	
}

//=================================================================================================
//! @brief		Load zbType::Manager
//! @details	Part of ManagerBase Interface.
//!-----------------------------------------------------------------------------
//! @return		True if init was successful
//=================================================================================================
bool ManagerExport::Load()
{	
	// Default callback for Getting new zenResID value, to use simple incrementing index (good for runtime resources)
	zenMem::Set(saNextID, 0, sizeof(saNextID) );		
	for( zenUInt idxPlatform(0); idxPlatform<zenConst::keResPlatform__Count; ++idxPlatform)
		for( zenUInt idxType(0); idxType<zenConst::keResType__Count; ++idxType)
			mpCallbackGetItemID[idxPlatform][idxType] = CallbackGetItemID;	
	
	// Configure specific callback for zenResID value 
#if AW_EXPORT_OR_RESOURCE_DX11	
	mpCallbackGetItemID[zenConst::keResPlatform_DX11][zenConst::keResType_GfxView]				= EExp::SerialGfxView_Base::CallbackGetItemID;
	mpCallbackGetItemID[zenConst::keResPlatform_DX11][zenConst::keResType_GfxMesh]				= EExp::SerialMesh_Base::CallbackGetItemID;
	mpCallbackGetItemID[zenConst::keResPlatform_DX11][zenConst::keResType_GfxSampler]			= EExp::SerialGfxSampler_DX11::CallbackGetItemID;
	mpCallbackGetItemID[zenConst::keResPlatform_DX11][zenConst::keResType_GfxBlend]				= EExp::SerialGfxBlend_DX11::CallbackGetItemID;
	mpCallbackGetItemID[zenConst::keResPlatform_DX11][zenConst::keResType_GfxDepthStencil]		= EExp::SerialGfxDepthStencil_DX11::CallbackGetItemID;
	mpCallbackGetItemID[zenConst::keResPlatform_DX11][zenConst::keResType_GfxRasterizer]		= EExp::SerialGfxRasterizer_DX11::CallbackGetItemID;
	mpCallbackGetItemID[zenConst::keResPlatform_DX11][zenConst::keResType_GfxShaderBinding]		= EExp::SerialShaderBinding_Base::CallbackGetItemID;
	mpCallbackGetItemID[zenConst::keResPlatform_DX11][zenConst::keResType_GfxInputStream]		= EExp::SerialGfxInputStream_DX11::CallbackGetItemID;
	mpCallbackGetItemID[zenConst::keResPlatform_DX11][zenConst::keResType_GfxInputSignature]	= EExp::SerialGfxInputSignature_DX11::CallbackGetItemID;	
	mpCallbackGetItemID[zenConst::keResPlatform_DX11][zenConst::keResType_GfxShaderVertex]		= EExp::SerialShader_Base::CallbackGetItemID;
	mpCallbackGetItemID[zenConst::keResPlatform_DX11][zenConst::keResType_GfxShaderPixel]		= EExp::SerialShader_Base::CallbackGetItemID;
	mpCallbackGetItemID[zenConst::keResPlatform_DX11][zenConst::keResType_GfxShaderParamDef]	= EExp::SerialGfxShaderParamDef_DX11::CallbackGetItemID;	
#endif

	return true;
}

void ManagerExport::SetExportInfos(zenConst::eResPlatform _aPlatforms[zenConst::keResPlatform__Count], zenConst::eResSource _eExportSource, CBCreateItem _pExportCBCreateItem )
{
	for(zenUInt platIdx(0); platIdx<zenConst::keResPlatform__Count; ++platIdx)
		maPlatforms[platIdx] = _aPlatforms[platIdx];
	meSource				= _eExportSource;
	mpCallbackCreateItem	= _pExportCBCreateItem;
}

zenResID ManagerExport::CreateItem( zenResID::ePlatformType _ePlatformType, zenConst::eResType _eResourceType, ExportInfoBase* _pExportInfoBase )
{ 
	ZENAssert(_ePlatformType<zenConst::keResPlatform__Count);
	ZENAssert(_eResourceType<zenConst::keResType__Count);
	ZENAssert(_pExportInfoBase);
	bool bExist(false);
	zenConst::eResPlatform eExportPlatform	= maPlatforms[_ePlatformType];
	_pExportInfoBase->mExportResID			= mpCallbackGetItemID[eExportPlatform][_eResourceType](eExportPlatform, _eResourceType, meSource, _pExportInfoBase, bExist);
	return bExist ? _pExportInfoBase->mExportResID : mpCallbackCreateItem(_pExportInfoBase); 
}

//=================================================================================================
//! @brief		Request an new zenResID for a particular resource type
//! @details	Use internal table, keeping track of each resource type zenResID creation 
//!				callback method.
//-------------------------------------------------------------------------------------------------
//! @return		New zenResID
//=================================================================================================
zenResID ManagerExport::GetNewResourceID(zenConst::eResPlatform _ePlatform, zenConst::eResType _eType, zenConst::eResSource _eSource, const ExportInfoBase* _pExportInfo, bool& _bExistOut)
{
	return mpCallbackGetItemID[_ePlatform][_eType](_ePlatform, _eType, _eSource, _pExportInfo, _bExistOut);
}

//=================================================================================================
//! @brief		Default zenResID naming scheme
//! @details	Keep track of a counter, and increment it for each time an ID is requested. 
//!				Useful for dynamically created resources
//-------------------------------------------------------------------------------------------------
//! @return		New zenResID
//=================================================================================================
zenResID ManagerExport::CallbackGetItemID(zenConst::eResPlatform _ePlatform, zenConst::eResType _eType, zenConst::eResSource _eSource, const ExportInfoBase* _pExportInfo, bool& _bExistOut)
{
	_bExistOut	= false;
	zenU32 uNextID = saNextID[_eType]++;
	return zenResID( uNextID, _ePlatform, _eType, _eSource );
}

//=================================================================================================
//! @brief		Unload zbType::Manager
//! @details	Part of ManagerBase Interface.
//--------------------------------------------------------------------------------------------------
//! @return		True if unload was successful
//=================================================================================================
bool ManagerExport::Unload()
{
	return true;
} 

//=================================================================================================
//! @brief		Has exporting task pending
//=================================================================================================
bool ManagerExport::HasPendingTask() const
{
	return muExportPending > 0;
}

//=================================================================================================
//! @brief		Wait until all export task have been processed
//! @details	
//-------------------------------------------------------------------------------------------------
//=================================================================================================
void ManagerExport::WaitExportDone() const
{
	while( HasPendingTask() )
	{
		CMgr::Job.Update();
		Sleep(1);
	}
}

void ManagerExport::ExportDone(ExportItem* _pExportItem)
{
	if( _pExportItem->mResID.IsExport() )
	{
		ZENAssert(muExportPending>0);
		--muExportPending;	
	}

	if( _pExportItem->mpExportInfo->IsSuccess() )	maExportSuccess.Append(_pExportItem);
	else											maExportFail.Append(_pExportItem);	
}

//=================================================================================================
//! @brief		Create a new ShaderInputSignature Resource
//! @details	
//-------------------------------------------------------------------------------------------------
//! @param _ParentShaderID	- ResourceId of the parent shader to generate the input signature for
//! @return 				- Unique zenResID of created Resource
//=================================================================================================
/*
zenResID CreateExportItemOffline(zenConst::eResType _eResType, EExp::ExportInfoBase* _pExportInfo)
{
	switch( EMgr::Export.GetExportGfx() )
	{
	#if AW_EXPORT_OR_RESOURCE_DX11
		case zenConst::keResPlatform_DX11:
		{
			switch(_eResType)
			{
			case zenConst::keResType_GfxIndexTemp: return zenNewDefaultEExp::SerialGfxIndex_DX11;
			}
		}break;
	#endif
	}
	switch( EMgr::Export.GetExportOS() )
	{
	#if AWEXPORT_WIN
		case zenConst::keResPlatform_Window:
		{			
		}break;			
	#endif
	}
	//EMgr::Export.Export( SerialIndex_Base::CreateSerialItem(), pExportInfo );
	//return pExportInfo->mExportResID;
	return NULL;	
}
*/

}
