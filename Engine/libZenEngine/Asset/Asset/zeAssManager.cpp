#include "libZenEngine.h"
#if ZEN_ENGINETOOL
namespace zeMgr{ zeAss::ManagerAsset Asset; }

namespace zen { namespace zeAss
{

//=================================================================================================
//! @brief		Constructor
//=================================================================================================
ManagerAsset::ManagerAsset()
: mdPackage(50)
, mdAsset(1000)
{	
	mdPackage.SetDefaultValue(NULL);
	mdAsset.SetDefaultValue(NULL);
}

//=================================================================================================
//! @brief		Load Manager
//! @details	Part of ManagerBase Interface.
//!-----------------------------------------------------------------------------
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
	// Process adding package
}

void ManagerAsset::PackageRename( zHash64 _hOldID, zHash64 _hNewID)
{
	zenAss::zPackage rPackage = mdPackage[_hOldID];
	if( rPackage.IsValid() )
	{		
		mdPackage.Unset(_hOldID);
		mdPackage.Set(_hNewID, rPackage);
	}	
}

void ManagerAsset::PackageRemove( zHash64 _hPackageID )	
{
	//! @todo Asset: Process removal
	ZENAssert(0);
	zenAss::zPackage rPackage = mdPackage[_hPackageID];
	if( rPackage.IsValid() )
	{		
		mdPackage.Unset(_hPackageID);
	}	
}

const zenAss::zPackage& ManagerAsset::PackageGet( zHash64 _hPackageID )
{
	return mdPackage[_hPackageID];
}

const zenAss::zAssetItem& ManagerAsset::AssetGet( zHash64 _hAssetID )
{
	return mdAsset[_hAssetID];
}

void ManagerAsset::PackageLoad()
{
	mdPackage.Clear();
	PackageLoad( "" );
}

void ManagerAsset::PackageLoad(const zString& _zDir)
{
	//---------------------------------------------------------------
	// Load all package file from currently loading directory
	//---------------------------------------------------------------
	const CFil::FileInfo* pFileInfo;
	CMgr::File.Search( CFil::keFileFlag_File, L"Packages", L"*.xml",  true);
	while( CMgr::File.SearchNext(pFileInfo) )
	{
		Package* pNewPackage = zenNewDefault Package;
		if( pNewPackage->Load(pFileInfo->GetFilename(), mdAsset) )	
			PackageAdd(pNewPackage);
		else
			zenDelNull(pNewPackage);
	}
}

}} //namespace zen { namespace zeAss

#endif //ZEN_ENGINETOOL
