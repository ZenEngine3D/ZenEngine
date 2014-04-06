#pragma once
#ifndef __LibZenEngine_Asset_Manager_h__
#define __LibZenEngine_Asset_Manager_h__

#if ZEN_ENGINETOOL
namespace zen { namespace zeAss
{	

class ManagerAsset : public zbType::Manager
{
ZENClassDeclare(ManagerAsset, zbType::Manager)
public:
											ManagerAsset	();
	void									PackageLoad		();
	
	void									PackageRename	( zHash64 _hOldID, zHash64 _hNewID);
	void									PackageRemove	( zHash64 _hPackageID );	
	const zenAss::zPackage&					PackageGet		( zHash64 _hPackageID );
	const zMap<zenAss::zPackage>::Key64&	PackageGet		()const{return mdPackage;}

	const zenAss::zAssetItem&				AssetGet		( zHash64 _hAssetID )const;
	const zMap<zenAss::zAssetItem>::Key64&	AssetGet		()const{return mdAsset;}
	
	void									AssetAdd		( zeAss::Asset* _pAsset );
	void									AssetRem		( zHash64 _hAssetID );

protected:
	void									PackageAdd		( zeAss::Package* _pPackage );

	void									PackageLoad		(const zString& _zDir);

	zMap<zenAss::zPackage>::Key64			mdPackage;					//!< All existing package
	zMap<zenAss::zAssetItem>::Key64			mdAsset;					//!< All existing assets
	
//---------------------------------------------------------
// ManagerBase Section
//---------------------------------------------------------
public:
	virtual	bool							Load();
	virtual	bool							Unload();
	friend class AssetLoader;
};	

void ResetAssetReference( zMap<zenAss::zAssetItem>::Key64& _dAssets, zenAss::zAssetItem& _rAssetDel);

}} //namespace zen { namespace zeAss

namespace zeMgr{ extern zeAss::ManagerAsset Asset; }

#endif
#endif