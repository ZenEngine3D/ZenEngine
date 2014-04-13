#include "libZenEngine.h"
#include <Engine/libZenExternal/libZenExternal.h>

#if ZEN_ENGINETOOL
namespace zeMgr{ zeAss::ManagerAsset Asset; }

namespace zen { namespace zeAss
{
//=================================================================================================
//! @brief		Called by AssetMap, when asset is removed from it
//! @details	Since no destructor is called on item removed from map, need to 
//!				decrement refcount manually, makifn sure there's no resource leak
//-------------------------------------------------------------------------------------------------
//! @return		True if init was successful
//=================================================================================================
void ResetAssetReference( zenAss::zArrayAsset& _dAsset, zenAss::zAssetItem& _rAssetDel)
{
	_rAssetDel = NULL;
}
void ResetPackageReference( zenAss::zArrayPackage& _dPackage, zenAss::zPackage& _rPackageDel)
{
	_rPackageDel = NULL;
}

//=================================================================================================
//! @brief		Constructor
//=================================================================================================
ManagerAsset::ManagerAsset()
: mdPackage(50)
, muPackageNextID(0)
{	
	mdPackage.SetDefaultValue(NULL);
	mdPackage.SetDeleteItemCB( ResetPackageReference );

	zenMem::Zero(maAssetNextID, sizeof(maAssetNextID) );
	for(zUInt idx(0); idx<zenConst::keAssType__Count; ++idx)
	{
		madAsset[idx].Init(1000);
		madAsset[idx].SetDefaultValue(NULL);
		madAsset[idx].SetDeleteItemCB( ResetAssetReference );			
	}
	
}

//=================================================================================================
//! @brief		Load Manager
//! @details	Part of ManagerBase Interface.
//-------------------------------------------------------------------------------------------------
//! @return		True if init was successful
//=================================================================================================
bool ManagerAsset::Load()
{		
	PackageLoad();
	return true;
}

//=================================================================================================
//! @brief		Unload Manager
//! @details	Part of ManagerBase Interface.
//-------------------------------------------------------------------------------------------------
//! @return		True if unload was successful
//=================================================================================================
bool ManagerAsset::Unload()
{	
	return true;
} 

void ManagerAsset::PackageAdd( Package* _pPackage )
{
	mdPackage.Set(_pPackage->GetID(), _pPackage);
	//! @todo Asset: Process adding package
}

void ManagerAsset::PackageLoad()
{
	zxAss::AssetLoaderXml xmlLoader;
	mdPackage.Clear();	
	xmlLoader.LoadPackages();

	zenAss::zArrayPackage::Iterator itPackage;
	mdPackage.GetLast(itPackage);
	muPackageNextID = itPackage.IsValid() ? itPackage.GetKey() + 1 : 1;

	zenAss::zArrayAsset::Iterator itAsset;
	for(zUInt idx(0); idx<zenConst::keAssType__Count; ++idx)
	{
		madAsset[idx].GetLast(itAsset);
		maAssetNextID[idx] = itAsset.IsValid() ? itAsset.GetKey() + 1 : 1;
	}


	//! @todo Clean: TEMP TEMP TEMP TEST
	//! @todo Asset : Remove Test Asset/Property
	//xmlLoader.SavePackages();	
}

bool ManagerAsset::PackageSave( zU32 _uPackageID )
{
	zxAss::AssetLoaderXml Loader;
	zenAss::zPackage rPackage = mdPackage[_uPackageID];
	if( rPackage.IsValid() )
		return Loader.Save(rPackage);
	return false;
}
/*
void ManagerAsset::PackageRename( zHash64 _hOldID, zHash64 _hNewID)
{
	zenAss::zPackage rPackage = mdPackage[_hOldID];
	if( rPackage.IsValid() )
	{		
		mdPackage.Unset(_hOldID);
		mdPackage.Set(_hNewID, rPackage);
	}	
}
*/
void ManagerAsset::PackageRemove( zU32 _uPackageID )	
{
	//! @todo Asset: Process removal
	ZENAssert(0);
	zenAss::zPackage rPackage = mdPackage[_uPackageID];
	if( rPackage.IsValid() )
	{		
		mdPackage.Unset(_uPackageID);
	}	
}

const zenAss::zPackage& ManagerAsset::PackageGet( zU32 _uPackageID )
{
	return mdPackage[_uPackageID];
}

const zenAss::zAssetItem& ManagerAsset::AssetGet( zenConst::eAssetType _eType, zU32 _uAssetID )const
{
	ZENAssert(_eType<zenConst::keAssType__Count); 
	return madAsset[_eType][_uAssetID];
}

const zenAss::zArrayAsset& ManagerAsset::AssetGet( zenConst::eAssetType _eType )const
{
	ZENAssert(_eType<zenConst::keAssType__Count); 
	return madAsset[_eType];
}

void ManagerAsset::AssetAdd( zeAss::Asset* _pAsset )
{
	ZENAssert( _pAsset && madAsset[_pAsset->GetType()].Exist(_pAsset->GetID()) == false );
	madAsset[_pAsset->GetType()].Set(_pAsset->GetID(), _pAsset);
}

void ManagerAsset::AssetRem( zenConst::eAssetType _eType, zU32 _uAssetID )
{
	ZENAssert(_eType<zenConst::keAssType__Count); 
	zenAss::zAssetItem rAsset;
	madAsset[_eType].Get(_uAssetID, rAsset);
	if( rAsset.IsValid() )
	{
		rAsset.Get()->SetPackage(NULL);
		madAsset[_eType].Unset(_uAssetID);
	}
}

}} //namespace zen { namespace zeAss

#endif //ZEN_ENGINETOOL
