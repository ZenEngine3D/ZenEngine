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
	void										PackageRename	( zenHash32 _hOldID, zenHash32 _hNewID);
	void										PackageRemove	( zenHash32 _hPackageID );	
	Package*									PackageGet		( zenHash32 _hPackageID );
	const zenMap<Package*>::Key32&				PackageGet		()const{return mdPackage;}

	AssetItem*									AssetGet		( zenHash64 _hAssetID );
	const zenMap<AssetItem*>::Key64&			AssetGet		()const{return mdAsset;}
protected:
	void										PackageLoad		(const zenString& _zDir);
	zenMap<Package*>::Key32					mdPackage;					//!< All existing package
	zenMap<AssetItem*>::Key64					mdAsset;					//!< All existing assets

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