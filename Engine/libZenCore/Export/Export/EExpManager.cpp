#include "libZenCore.h"

namespace EMgr { EExp::ManagerExport Export; }

namespace EExp
{

awU32	ManagerExport::saNextID[awconst::keResType__Count];

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
	// Default callback for Getting new awResourceID value, to use simple incrementing index (good for runtime resources)
	memset(saNextID, 0, sizeof(saNextID) );		
	for( awUInt idxPlatform(0); idxPlatform<awconst::keResPlatform__Count; ++idxPlatform)
		for( awUInt idxType(0); idxType<awconst::keResType__Count; ++idxType)
			mpCallbackGetItemID[idxPlatform][idxType] = CallbackGetItemID;	
	
	// Configure specific callback for awResourceID value 
#if AW_EXPORT_OR_RESOURCE_DX11	
	mpCallbackGetItemID[awconst::keResPlatform_DX11][awconst::keResType_GfxView]					= EExp::SerialGfxView_Base::CallbackGetItemID;
	mpCallbackGetItemID[awconst::keResPlatform_DX11][awconst::keResType_GfxMesh]					= EExp::SerialMesh_Base::CallbackGetItemID;
	mpCallbackGetItemID[awconst::keResPlatform_DX11][awconst::keResType_GfxSampler]					= EExp::SerialGfxSampler_DX11::CallbackGetItemID;
	mpCallbackGetItemID[awconst::keResPlatform_DX11][awconst::keResType_GfxBlend]					= EExp::SerialGfxBlend_DX11::CallbackGetItemID;
	mpCallbackGetItemID[awconst::keResPlatform_DX11][awconst::keResType_GfxDepthStencil]			= EExp::SerialGfxDepthStencil_DX11::CallbackGetItemID;
	mpCallbackGetItemID[awconst::keResPlatform_DX11][awconst::keResType_GfxRasterizer]				= EExp::SerialGfxRasterizer_DX11::CallbackGetItemID;
	mpCallbackGetItemID[awconst::keResPlatform_DX11][awconst::keResType_GfxShaderBinding]			= EExp::SerialShaderBinding_Base::CallbackGetItemID;
	mpCallbackGetItemID[awconst::keResPlatform_DX11][awconst::keResType_GfxInputStream]				= EExp::SerialGfxInputStream_DX11::CallbackGetItemID;
	mpCallbackGetItemID[awconst::keResPlatform_DX11][awconst::keResType_GfxInputSignature]			= EExp::SerialGfxInputSignature_DX11::CallbackGetItemID;	
	mpCallbackGetItemID[awconst::keResPlatform_DX11][awconst::keResType_GfxShaderVertex]			= EExp::SerialShader_Base::CallbackGetItemID;
	mpCallbackGetItemID[awconst::keResPlatform_DX11][awconst::keResType_GfxShaderPixel]				= EExp::SerialShader_Base::CallbackGetItemID;
	mpCallbackGetItemID[awconst::keResPlatform_DX11][awconst::keResType_GfxShaderParamDef]			= EExp::SerialGfxShaderParamDef_DX11::CallbackGetItemID;	
#endif

	return true;
}

void ManagerExport::SetExportInfos(awconst::eResPlatform _aPlatforms[awconst::keResPlatform__Count], awconst::eResSource _eExportSource, CBCreateItem _pExportCBCreateItem )
{
	for(awUInt platIdx(0); platIdx<awconst::keResPlatform__Count; ++platIdx)
		maPlatforms[platIdx] = _aPlatforms[platIdx];
	meSource				= _eExportSource;
	mpCallbackCreateItem	= _pExportCBCreateItem;
}

awResourceID ManagerExport::CreateItem( awResourceID::ePlatformType _ePlatformType, awconst::eResType _eResourceType, ExportInfoBase* _pExportInfoBase )
{ 
	AWAssert(_ePlatformType<awconst::keResPlatform__Count);
	AWAssert(_eResourceType<awconst::keResType__Count);
	AWAssert(_pExportInfoBase);
	bool bExist(false);
	awconst::eResPlatform eExportPlatform	= maPlatforms[_ePlatformType];
	_pExportInfoBase->mExportResID			= mpCallbackGetItemID[eExportPlatform][_eResourceType](eExportPlatform, _eResourceType, meSource, _pExportInfoBase, bExist);
	return bExist ? _pExportInfoBase->mExportResID : mpCallbackCreateItem(_pExportInfoBase); 
}

//=================================================================================================
//! @brief		Request an new awResourceID for a particular resource type
//! @details	Use internal table, keeping track of each resource type awResourceID creation 
//!				callback method.
//-------------------------------------------------------------------------------------------------
//! @return		New awResourceID
//=================================================================================================
awResourceID ManagerExport::GetNewResourceID(awconst::eResPlatform _ePlatform, awconst::eResType _eType, awconst::eResSource _eSource, const ExportInfoBase* _pExportInfo, bool& _bExistOut)
{
	return mpCallbackGetItemID[_ePlatform][_eType](_ePlatform, _eType, _eSource, _pExportInfo, _bExistOut);
}

//=================================================================================================
//! @brief		Default awResourceID naming scheme
//! @details	Keep track of a counter, and increment it for each time an ID is requested. 
//!				Useful for dynamically created resources
//-------------------------------------------------------------------------------------------------
//! @return		New awResourceID
//=================================================================================================
awResourceID ManagerExport::CallbackGetItemID(awconst::eResPlatform _ePlatform, awconst::eResType _eType, awconst::eResSource _eSource, const ExportInfoBase* _pExportInfo, bool& _bExistOut)
{
	_bExistOut	= false;
	awU32 uNextID = saNextID[_eType]++;
	return awResourceID( uNextID, _ePlatform, _eType, _eSource );
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
		AWAssert(muExportPending>0);
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
//! @return 				- Unique awResourceID of created Resource
//=================================================================================================
/*
awResourceID CreateExportItemOffline(awconst::eResType _eResType, EExp::ExportInfoBase* _pExportInfo)
{
	switch( EMgr::Export.GetExportGfx() )
	{
	#if AW_EXPORT_OR_RESOURCE_DX11
		case awconst::keResPlatform_DX11:
		{
			switch(_eResType)
			{
			case awconst::keResType_GfxIndexTemp: return AWNewDefaultEExp::SerialGfxIndex_DX11;
			}
		}break;
	#endif
	}
	switch( EMgr::Export.GetExportOS() )
	{
	#if AWEXPORT_WIN
		case awconst::keResPlatform_Window:
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
