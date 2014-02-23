#pragma once
#if !defined(__LibAsset_Asset_Manager_h__) && AW_ENGINETOOL
#define __LibAsset_Asset_Manager_h__

namespace FAss
{	

class ManagerAsset : public Manager
{
AWClassDeclare(ManagerAsset, Manager)
public:
												ManagerAsset	();
	void										PackageLoad		();
	void										PackageAdd		( Package* _pPackage );
	void										PackageRename	( awHash32 _hOldID, awHash32 _hNewID);
	void										PackageRemove	( awHash32 _hPackageID );	
	Package*									PackageGet		( awHash32 _hPackageID );
	const Hashmap<Package*>::Key32&				PackageGet		()const{return mdPackage;}

	AssetItem*									AssetGet		( awHash64 _hAssetID );
	const Hashmap<AssetItem*>::Key64&			AssetGet		()const{return mdAsset;}
protected:
	void										PackageLoad		(const awString& _zDir);
	Hashmap<Package*>::Key32					mdPackage;					//!< All existing package
	Hashmap<AssetItem*>::Key64					mdAsset;					//!< All existing assets

//---------------------------------------------------------
// ManagerBase Section
//---------------------------------------------------------
public:
	virtual	bool					Load();
	virtual	bool					Unload();
};	
}

namespace FMgr{ extern FAss::ManagerAsset Asset; }

#endif