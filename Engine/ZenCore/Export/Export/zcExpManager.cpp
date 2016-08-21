#include "zcCore.h"

namespace zcMgr { zcExp::ManagerExport Export; }
namespace zcRes { zResID RuntimeCreateResource(zcExp::ExportInfoBase* _pExportInfo); }

namespace zcExp
{
zU32	ManagerExport::saNextID[zenConst::keResType__Count];

//=================================================================================================
//! @brief		Constructor
//=================================================================================================
ManagerExport::ManagerExport()
{	
	
	zenConst::eResPlatform aPlatformTypes[zResID::kePlatformType__Count];
	aPlatformTypes[zResID::kePlatformType_OS]	= zenConst::kCurrentPlatformOS;
	aPlatformTypes[zResID::kePlatformType_GFX]	= zenConst::kCurrentPlatformGfx;
	SetExportInfos( aPlatformTypes, zenConst::keResSource_Runtime, zcRes::RuntimeCreateResource ); //! @todo Clean support export/runtime creation
}

//=================================================================================================
//! @brief		Load zbType::Manager
//! @details	Part of ManagerBase Interface.
//!-----------------------------------------------------------------------------
//! @return		True if init was successful
//=================================================================================================
bool ManagerExport::Load()
{	
	// Default callback for Getting new zResID value, to use simple incrementing index (good for runtime resources)
	zenMem::Zero(saNextID, sizeof(saNextID) );		
	for( zUInt idxPlatform(0); idxPlatform<zenConst::keResPlatform__Count; ++idxPlatform)
		for( zUInt idxType(0); idxType<zenConst::keResType__Count; ++idxType)
			mpCallbackGetItemID[idxPlatform][idxType] = CallbackGetItemID;	
	
	// Configure specific callback for zResID value 
	mpCallbackGetItemID[zenConst::keResPlatform_DX11][zenConst::keResType_GfxView]				= zcExp::ExportInfoGfxView::CallbackGetItemID;
	mpCallbackGetItemID[zenConst::keResPlatform_DX11][zenConst::keResType_GfxMesh]				= zcExp::ExportInfoGfxMesh::CallbackGetItemID;
	mpCallbackGetItemID[zenConst::keResPlatform_DX11][zenConst::keResType_GfxShaderVertex]		= zcExp::ExportInfoGfxShader::CallbackGetItemID;
	mpCallbackGetItemID[zenConst::keResPlatform_DX11][zenConst::keResType_GfxShaderPixel]		= zcExp::ExportInfoGfxShader::CallbackGetItemID;
	mpCallbackGetItemID[zenConst::keResPlatform_DX11][zenConst::keResType_GfxShaderParamDef]	= zcExp::ExportInfoGfxShaderParamDef::CallbackGetItemID;
	mpCallbackGetItemID[zenConst::keResPlatform_DX11][zenConst::keResType_GfxShaderBinding]		= zcExp::ExportInfoGfxShaderBinding::CallbackGetItemID;
	mpCallbackGetItemID[zenConst::keResPlatform_DX11][zenConst::keResType_GfxInputStream]		= zcExp::ExporterGfxInputStream::CallbackGetItemID;
	//! @todo cleanup move this to console specific code
#if ZEN_RENDERER_DX11		
	mpCallbackGetItemID[zenConst::keResPlatform_DX11][zenConst::keResType_GfxSampler]			= zcExp::ExporterGfxSamplerDX11_DX11::CallbackGetItemID;
	mpCallbackGetItemID[zenConst::keResPlatform_DX11][zenConst::keResType_GfxBlend]				= zcExp::ExporterGfxStateBlendDX11_DX11::CallbackGetItemID;
	mpCallbackGetItemID[zenConst::keResPlatform_DX11][zenConst::keResType_GfxDepthStencil]		= zcExp::ExporterGfxStateDepthStencilDX11_DX11::CallbackGetItemID;
	mpCallbackGetItemID[zenConst::keResPlatform_DX11][zenConst::keResType_GfxRaster]			= zcExp::ExporterGfxStateRasterDX11_DX11::CallbackGetItemID;		
	mpCallbackGetItemID[zenConst::keResPlatform_DX11][zenConst::keResType_GfxInputSignature]	= zcExp::ExporterGfxInputSignatureDX11_DX11::CallbackGetItemID;		
#endif

	return true;
}

void ManagerExport::SetExportInfos(zenConst::eResPlatform _aPlatforms[zenConst::keResPlatform__Count], zenConst::eResSource _eExportSource, CBCreateItem _pExportCBCreateItem )
{
	for(zUInt platIdx(0); platIdx<zenConst::keResPlatform__Count; ++platIdx)
		maPlatforms[platIdx] = _aPlatforms[platIdx];
	meSource				= _eExportSource;
	mpCallbackCreateItem	= _pExportCBCreateItem;
}
//! @todo Clean, remove platformtype, use resourcetype to know
zResID ManagerExport::CreateItem( zResID::ePlatformType _ePlatformType, zenConst::eResType _eResourceType, ExportInfoBase* _pExportInfoBase )
{ 
	zenAssert(_ePlatformType<zenConst::keResPlatform__Count);
	zenAssert(_eResourceType<zenConst::keResType__Count);
	zenAssert(_pExportInfoBase);
	bool bExist(false);
	zenConst::eResPlatform eExportPlatform	= maPlatforms[_ePlatformType];
	_pExportInfoBase->mExportResID			= mpCallbackGetItemID[eExportPlatform][_eResourceType](eExportPlatform, _eResourceType, meSource, _pExportInfoBase, bExist);
	zResID NewResID							= bExist ? _pExportInfoBase->mExportResID : mpCallbackCreateItem(_pExportInfoBase);
	zenDel(_pExportInfoBase); //! @todo safely manage this better. RefCount? Stack Allocate without pool?
	return NewResID;
}

//=================================================================================================
//! @brief		Request a new zResID for a particular resource type
//! @details	Use internal table, keeping track of each resource type zResID creation 
//!				callback method.
//-------------------------------------------------------------------------------------------------
//! @return		New zResID
//=================================================================================================
zResID ManagerExport::GetNewResourceID(zenConst::eResPlatform _ePlatform, zenConst::eResType _eType, zenConst::eResSource _eSource, const ExportInfoBase* _pExportInfo, bool& _bExistOut)
{
	return mpCallbackGetItemID[_ePlatform][_eType](_ePlatform, _eType, _eSource, _pExportInfo, _bExistOut);
}

//=================================================================================================
//! @brief		Request a new zResID for a resource type created at runtime
//! @details	
//-------------------------------------------------------------------------------------------------
//! @return		New zResID
//=================================================================================================
zResID ManagerExport::GetNewResourceID(zenConst::eResType _eType)
{
	bool _bUnused;
	return CallbackGetItemID(zenConst::kCurrentPlatformOS, _eType, zenConst::keResSource_Runtime, nullptr, _bUnused);
}

//=================================================================================================
//! @brief		Default zResID naming scheme
//! @details	Keep track of a counter, and increment it for each time an ID is requested. 
//!				Useful for dynamically created resources
//-------------------------------------------------------------------------------------------------
//! @return		New zResID
//=================================================================================================
zResID ManagerExport::CallbackGetItemID(zenConst::eResPlatform _ePlatform, zenConst::eResType _eType, zenConst::eResSource _eSource, const ExportInfoBase* _pExportInfo, bool& _bExistOut)
{
	_bExistOut	= false;
	zU32 uNextID = saNextID[_eType]++;
	return zResID( uNextID, _ePlatform, _eType, _eSource );
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

#if 0
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
	//	zcMgr::Job.Update();
		Sleep(1);
	}
}
#endif

}
