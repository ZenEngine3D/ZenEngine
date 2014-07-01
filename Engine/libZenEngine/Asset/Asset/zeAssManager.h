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
	
	void									PackageRemove	( zU32 _uPackageID );	
	const zenAss::zPackage&					PackageGet		( zU32 _uPackageID );
	bool									PackageSave		( zU32 _uPackageID );
	const zenAss::zArrayPackage&			PackageGet		()const{return mdPackage;}
	
	const zenAss::zAssetItem&				AssetGet		( zenConst::eAssetType _eType, zU32 _hAssetID )const;
	const zenAss::zArrayAsset&				AssetGet		( zenConst::eAssetType _eType )const;
	
	void									AssetAdd		( zeAss::Asset* _pAsset );
	void									AssetRem		( zenAss::zAssetID _AssetID );
	ZENInline zenAss::zAssetID				GetAssetNextID	( zenConst::eAssetType _eType){ ZENAssert(_eType < zenConst::keAssType__Count); return zenAss::zAssetID(_eType, maAssetNextID[_eType]++);};
	ZENInline zU32							GetPackageNextID(){return muPackageNextID++;};
protected:
	void									PackageAdd		( zeAss::Package* _pPackage );

	zenAss::zArrayPackage					mdPackage;									//!< All existing package
	zenAss::zArrayAsset						madAsset[zenConst::keAssType__Count];		//!< All existing assets
	zU32									muPackageNextID;
	zU32									maAssetNextID[zenConst::keAssType__Count];
//---------------------------------------------------------
// ManagerBase Section
//---------------------------------------------------------
public:
	virtual	bool							Load();
	virtual	bool							Unload();
	friend class AssetLoader;
};	

void ResetAssetReference( zenAss::zArrayAsset& _dAssets, zenAss::zAssetItem& _rAssetDel);

}} //namespace zen { namespace zeAss

namespace zeMgr{ extern zeAss::ManagerAsset Asset; }

#endif
#endif