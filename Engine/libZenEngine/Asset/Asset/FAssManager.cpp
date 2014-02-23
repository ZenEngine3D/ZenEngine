#include "libZenEngine.h"
#if AW_ENGINETOOL
namespace FMgr{ FAss::ManagerAsset Asset; }

namespace FAss
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

void ManagerAsset::PackageRename( awHash32 _hOldID, awHash32 _hNewID)
{
	Package* pPackage = mdPackage[_hOldID];
	if( pPackage )
	{		
		mdPackage.Unset(_hOldID);
		mdPackage.Set(_hNewID, pPackage);
	}	
}

void ManagerAsset::PackageRemove( awHash32 _hPackageID )	
{
	//! @todo : Process removal
	AWAssert(0);
	Package* pPackage = mdPackage[_hPackageID];
	if( pPackage )
	{		
		mdPackage.Unset(_hPackageID);
	}	
}

Package* ManagerAsset::PackageGet( awHash32 _hPackageID )
{
	return mdPackage[_hPackageID];
}

AssetItem* ManagerAsset::AssetGet( awHash64 _hAssetID )
{
	return mdAsset[_hAssetID];
}

void ManagerAsset::PackageLoad()
{
	mdPackage.Clear();
	PackageLoad( "" );
}

void ManagerAsset::PackageLoad(const awString& _zDir)
{
	//---------------------------------------------------------------
	// Load all package file from currently loading directory
	//---------------------------------------------------------------
	const CFil::FileInfo* pFileInfo;
	CMgr::File.Search( CFil::keFileFlag_File, L"Packages", L"*.xml",  true);
	while( CMgr::File.SearchNext(pFileInfo) )
	{
		Package* pNewPackage = AWNewDefault Package;
		if( pNewPackage->Load(pFileInfo->GetFilename(), mdAsset) )	
			PackageAdd(pNewPackage);
		else
			AWDelNull(pNewPackage);
	}
}

}

#endif //AW_ENGINETOOL
