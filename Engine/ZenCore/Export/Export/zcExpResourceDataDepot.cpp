#include "zcCore.h"

namespace zcDepot{ zcExp::DepotResourceData ExportData; }

namespace zcExp
{

const zenConst::eEngineVersion gResourceVersions[]=
{
	//--- GFX ---
	zenConst::keEngineVersion_Initial,		//keResType_GfxShaderPixel,
	zenConst::keEngineVersion_Initial,		//keResType_GfxShaderVertex,
	zenConst::keEngineVersion_Initial,		//keResType_GfxShaderParamDef,
	zenConst::keEngineVersion_Initial,		//keResType_GfxShaderParam,
	zenConst::keEngineVersion_Initial,		//keResType_GfxShaderBinding,
	zenConst::keEngineVersion_Initial,		//keResType_GfxRenderTarget,
	zenConst::keEngineVersion_Initial,		//keResType_GfxView,
	zenConst::keEngineVersion_Initial,		//keResType_GfxWindow,
	zenConst::keEngineVersion_Initial,		//keResType_GfxIndex,
	zenConst::keEngineVersion_Initial,		//keResType_GfxVertex,
	zenConst::keEngineVersion_Initial,		//keResType_GfxBuffer,
	zenConst::keEngineVersion_Initial,		//keResType_GfxTexture2D,
	zenConst::keEngineVersion_Initial,		//keResType_GfxSampler,
	zenConst::keEngineVersion_Initial,		//keResType_GfxBlend,
	zenConst::keEngineVersion_Initial,		//keResType_GfxDepthStencil,
	zenConst::keEngineVersion_Initial,		//keResType_GfxRasterizer,
	zenConst::keEngineVersion_Initial,		//keResType_GfxMeshStrip,
	zenConst::keEngineVersion_Initial,		//keResType_GfxMesh,
	zenConst::keEngineVersion_Initial,		//keResType_GfxInputStream,
	zenConst::keEngineVersion_Initial,		//keResType_GfxInputSignature,
	zenConst::keEngineVersion_Initial,		//keResType_GfxRenderPass,
};

void ResetResourceDataReference(zMap<zEngineRef<ResourceData>>::Key64& _dResData, zEngineRef<ResourceData>& _rResDataDel)
{
	_rResDataDel = nullptr;
}

//=================================================================================================
//! @brief		Constructor
//=================================================================================================
DepotResourceData::DepotResourceData()
//: mdResourceData(256)
{	
	zenStaticAssert( ZENArrayCount(gResourceVersions)==zenConst::keResType__Count );
}

//=================================================================================================
//! @brief		Load zbType::Manager
//! @details	Part of ManagerBase Interface.
//!-----------------------------------------------------------------------------
//! @return		True if init was successful
//=================================================================================================
bool DepotResourceData::Load()
{	
	//mdResourceData.SetDefaultValue(nullptr);
	//mdResourceData.SetDeleteItemCB(ResetResourceDataReference);
	return true;
}

//=================================================================================================
//! @brief		Unload zbType::Manager
//! @details	Part of ManagerBase Interface.
//--------------------------------------------------------------------------------------------------
//! @return		True if unload was successful
//=================================================================================================
bool DepotResourceData::Unload()
{
	return true;
} 

void DepotResourceData::Set(zenRes::zExportData* _pExportData)
{
	zenAssert(_pExportData);
	mdResourceData[_pExportData->mResID.GetHashID()] = _pExportData;
}

void DepotResourceData::Remove(const zResID& _ResID)
{
	zenAssert(_ResID.IsValid());
	mdResourceData.erase(_ResID.GetHashID());
}

//=================================================================================================
//! @brief		
//! @details	
//-------------------------------------------------------------------------------------------------
//! @return 	
//=================================================================================================
zEngineRef<zenRes::zExportData> DepotResourceData::Get(const zResID& _ResID)
{
	zEngineRef<zenRes::zExportData> rExportData;
	if( _ResID.IsValid() )
		rExportData = mdResourceData[_ResID.GetHashID()];
	return rExportData;
}

//=================================================================================================
//! @brief		Find and return the ResourceData with a particular zResID
//! @details	This version try to look in all possible ResourceData Sources, not limited to  
//!				the one provided in the zResID parameter.
//!				Some generated zResID can be created on the fly or already loaded, useful
//!				for those cases where we create a resource that depends on another one
//-------------------------------------------------------------------------------------------------
//! @param		_ResID				- zResID to look for
//! @return 	Found export data
zEngineRef<zenRes::zExportData> DepotResourceData::GetAnySource(const zResID& _ResID)
{
	zenAssert(_ResID.IsValid());
	zResID anySourceResID(_ResID);

	anySourceResID.SetSource(zenConst::keResSource_Loaded);
	auto itemIt = mdResourceData.find( _ResID.GetHashID() );
	if( itemIt != mdResourceData.end() )
		return itemIt->second;

	anySourceResID.SetSource(zenConst::keResSource_Runtime);
	itemIt = mdResourceData.find(_ResID.GetHashID());
	if (itemIt != mdResourceData.end())
		return itemIt->second;

	anySourceResID.SetSource(zenConst::keResSource_Offline);
	itemIt = mdResourceData.find(_ResID.GetHashID());
	if (itemIt != mdResourceData.end())
		return itemIt->second;

	return nullptr;
}
	
	
	


//=================================================================================================
//! @brief		Tell us if the resource exist and is valid
//! @details	Useful at export, to ensure all resource reference are valid
//-------------------------------------------------------------------------------------------------
//! @param		_ResID				- zResID to look for
//! @return 	true if valid
//=================================================================================================
bool DepotResourceData::IsValid(const zResID& _ResID)
{
	return	mdResourceData.find(_ResID.GetHashID()) != mdResourceData.end();
}

//=================================================================================================
//! @brief		Tell us if the resources exist and are valid
//! @details	Useful at export, to ensure all resource reference are valid
//-------------------------------------------------------------------------------------------------
//! @param		_ResID				- zResID to look for
//! @return 	true if valid
//=================================================================================================
bool DepotResourceData::IsValid(const zArrayBase<zResID>& _aResID)
{
	const zResID* pResIdCur	= _aResID.First();
	const zResID* pResIDEnd	= _aResID.Last()+1;
	while( pResIdCur < pResIDEnd )
	{
		if( mdResourceData.find(pResIdCur->GetHashID()) == mdResourceData.end() )
			return false;
		++pResIdCur;
	}
	return true;
}

zenConst::eEngineVersion DepotResourceData::GetEngineVersion(zenConst::eResType _eResType) const
{
	zenAssert(_eResType < zenConst::keResType__Count);
	return gResourceVersions[_eResType];
}

}
