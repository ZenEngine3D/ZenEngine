#pragma once
#ifndef __LibZenEngine_Asset_Manager_h__
#define __LibZenEngine_Asset_Manager_h__

#if ZEN_ENGINETOOL
namespace zeAss
{	

class ManagerAsset : public zbType::Manager
{
zenClassDeclare(ManagerAsset, zbType::Manager)
public:
												ManagerAsset();
	void										PackageLoad();
	
	zenAss::zPackageGroupRef					GroupCreate( const zWString& _zName, const zenAss::zPackageGroupRef& _rParent);
	ZENInline const zenAss::zPackageGroupRef&	GroupGetRoot()const;
	void										GroupDelete( const zenAss::zPackageGroupRef& _rGroup );

	ZENInline const zenAss::zArrayPackage&		PackageGet()const;
	const zenAss::zPackageRef&					PackageGet( zU32 _uPackageID );
	bool										PackageSave(const zenAss::zPackageRef& _rPackage);
	void										PackageDelete( const zenAss::zPackageRef& _rPackage );	
	void										PackageAdd( const zenAss::zPackageRef& _rPackage );

	const zenAss::zAssetItemRef&				AssetGet( zenConst::eAssetType _eType, zU32 _hAssetID )const;
	const zenAss::zArrayAsset&					AssetGet( zenConst::eAssetType _eType )const;
	
	void										AssetAdd( const zenAss::zAssetItemRef& _rAsset );
	void										AssetDelete( zenAss::zAssetItemRef _rAsset ); 
	ZENInline zenAss::zAssetID					GetAssetNextID( zenConst::eAssetType _eType);
	ZENInline zU32								GetPackageNextID();

	bool										SaveAll();
	ZENInline bool								IsLoading();
protected:
	

	zenAss::zPackageGroupRef					mrGroupRoot;
	zenAss::zArrayPackage						mdPackage;									//!< All existing package
	zenAss::zArrayAsset							madAsset[zenConst::keAssType__Count];		//!< All existing assets
	zU32										muPackageNextID;
	zU32										maAssetNextID[zenConst::keAssType__Count];
	bool										mbLoading;									//!< Currently loading packages
//---------------------------------------------------------
// ManagerBase Section
//---------------------------------------------------------
public:
	virtual	bool								Load();
	virtual	bool								Unload();
	//friend class AssetSerializer;
};	

void ResetAssetReference( zenAss::zArrayAsset& _dAssets, zenAss::zAssetItemRef& _rAssetDel);

} //namespace zeAss

#include "zeAssManager.inl"

namespace zeMgr{ extern zeAss::ManagerAsset Asset; }

#endif
#endif