#include "zcCore.h"

namespace zcDepot{ zcExp::DepotResourceData ResourceData; }

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
	ZENStaticAssert( ZENArrayCount(gResourceVersions)==zenConst::keResType__Count );	
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

//=================================================================================================
//! @brief		Store a new ResourceData 
//! @details	If a previous serial item existed, will remove it first
//=================================================================================================
void DepotResourceData::SetItem(const zEngineRef<ResourceData>& _rResData)
{
	ZENAssert(_rResData.IsValid());
	//mdResourceData.Set(_rResData->mResID.GetHashID(), _rResData);
	mdResourceData[_rResData->mResID.GetHashID()] = _rResData;
} 

void DepotResourceData::ClearItem(const zResID& _ResID)
{
	ZENAssert(_ResID.IsValid());
	//mdResourceData.Unset(_ResID.GetHashID());
	mdResourceData.erase(_ResID.GetHashID());
}

//=================================================================================================
//! @brief		
//! @details	
//-------------------------------------------------------------------------------------------------
//! @return 	
//=================================================================================================
zEngineConstRef<ResourceData> DepotResourceData::GetItemBase(const zResID& _ResID)
{
	zEngineRef<ResourceData> rResData;
	//mdResourceData.Get( _ResID.GetHashID(), rResData );
	rResData = mdResourceData[_ResID.GetHashID()];
	return rResData;
} 

//=================================================================================================
//! @brief		Find and return the ResourceData with a particular zResID
//! @details	This version try to look in all possible ResourceData Sources, not limited to  
//!				the one provided in the zResID parameter.
//!				Some generated zResID can be created on the fly or already loaded, useful
//!				for those cases where we create a resource that depends on another one
//-------------------------------------------------------------------------------------------------
//! @param		_ResID				- zResID to look for
//! @return 	void* to ResourceData
//=================================================================================================
zEngineConstRef<ResourceData> DepotResourceData::GetItemBaseAnySource(const zResID& _ResID)
{
	ZENAssert(_ResID.IsValid());
	zEngineRef<ResourceData> rResData;
	zResID anySourceResID(_ResID);

	std::map<zU64, zEngineRef<ResourceData>, std::less<zU64>>::iterator itemIt;
	anySourceResID.SetSource(zenConst::keResSource_Loaded);
	itemIt = mdResourceData.find( _ResID.GetHashID() );
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

	/*
	anySourceResID.SetSource(zenConst::keResSource_Loaded);
	if( mdResourceData.Get(_ResID.GetHashID(), rResData) )	
		return rResData;

	anySourceResID.SetSource(zenConst::keResSource_Runtime);	
	if( mdResourceData.Get(_ResID.GetHashID(), rResData) )	
		return rResData;
	
	anySourceResID.SetSource(zenConst::keResSource_Offline);	
	if( mdResourceData.Get(_ResID.GetHashID(), rResData) )	
		return rResData;
	*/
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
	return mdResourceData.find(_ResID.GetHashID()) != mdResourceData.end();
	//return mdResourceData.Exist(_ResID.GetHashID());
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
		//if( !mdResourceData.Exist(pResIdCur->GetHashID()) )
		if( mdResourceData.find(pResIdCur->GetHashID()) == mdResourceData.end() )
			return false;
		++pResIdCur;
	}
	return true;
}

zenConst::eEngineVersion DepotResourceData::GetEngineVersion(zenConst::eResType _eResType) const
{
	ZENAssert(_eResType < zenConst::keResType__Count);
	return gResourceVersions[_eResType];
}

}
