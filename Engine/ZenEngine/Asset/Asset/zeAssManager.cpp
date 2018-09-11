#include "zeEngine.h"
#include <ZenExternal/zxExternal.h>

#if ZEN_ENGINETOOL
namespace zeMgr{ zeAss::ManagerAsset Asset; }

namespace zeAss
{
//=================================================================================================
//! @brief		Called by AssetMap, when asset is removed from it
//! @details	Since no destructor is called on item removed from map, need to 
//!				decrement refcount manually, making sure there's no resource leak
//-------------------------------------------------------------------------------------------------
//! @return		True if init was successful
//=================================================================================================
void ResetAssetReference( zenAss::zArrayAsset& _dAsset, zenAss::zAssetItemRef& _rAssetDel)
{
	_rAssetDel = nullptr;
}
void ResetPackageReference( zenAss::zArrayPackage& _dPackage, zenAss::zPackageRef& _rPackageDel)
{
	_rPackageDel = nullptr;
}

//=================================================================================================
//! @brief		Constructor
//=================================================================================================
ManagerAsset::ManagerAsset()
: mdPackage(50)
, muPackageNextID(0)
, mbLoading(false)
{	
	mdPackage.SetDefaultValue(nullptr);
	mdPackage.SetDeleteItemCB( ResetPackageReference );

	zenMem::Zero(maAssetNextID, sizeof(maAssetNextID) );
	for(zUInt idx(0); idx<zenConst::keAssType__Count; ++idx)
	{
		madAsset[idx].Init(1000);
		madAsset[idx].SetDefaultValue(nullptr);
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
	mrGroupRoot	= zenNew zeAss::PackageGroup(zenT("Root"), nullptr );
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

//=================================================================================================
//! @brief		Add a new Package Group
//! @details	Create a new Group, where we'll be able to store child packages 
//!				and other subgroups
//-------------------------------------------------------------------------------------------------
//! @param _rParent	-	Parent of the group to create (Root if none specified)
//! @param _zName -		Names assigned to group
//! @return				Group created
//=================================================================================================
zenAss::zPackageGroupRef ManagerAsset::GroupCreate( const zWString& _zName, const zenAss::zPackageGroupRef& _rParent )
{
	zenAss::zPackageGroupRef rParent = _rParent.IsValid() ? _rParent : mrGroupRoot;
	return zenNew zeAss::PackageGroup(_zName, _rParent);
}

void ManagerAsset::GroupDelete( const zenAss::zPackageGroupRef& _rGroup )
{
	zxAss::AssetLoaderXml xmlLoader;
	xmlLoader.Delete(_rGroup);
}

void ManagerAsset::PackageAdd( const zenAss::zPackageRef& _rPackage )
{
	mdPackage.Set(_rPackage.GetID(), _rPackage);
	//! @todo Asset: Process adding package
}

void ManagerAsset::PackageLoad()
{
	mbLoading = true;
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
		maAssetNextID[idx] = itAsset.IsValid() ? itAsset.GetValue().GetID().muIndex + 1 : 1;
	}
	mbLoading = false;
}

void ManagerAsset::PackageDelete( const zenAss::zPackageRef& _rPackage )	
{
	zxAss::AssetLoaderXml Loader;
	if( _rPackage.IsValid() )
	{
		Loader.Delete( _rPackage );
		_rPackage->Unload();		
		mdPackage.Unset(_rPackage.GetID());		
	}
}

const zenAss::zPackageRef& ManagerAsset::PackageGet( zU32 _uPackageID )
{
	return mdPackage[_uPackageID];
}

const zenAss::zAssetItemRef& ManagerAsset::AssetGet( zenConst::eAssetType _eType, zU32 _uAssetID )const
{
	static const zenAss::zAssetItemRef sInvalid;
	if( _eType<zenConst::keAssType__Count )
		return madAsset[_eType][_uAssetID];
	else
		return sInvalid;
}

const zenAss::zArrayAsset& ManagerAsset::AssetGet( zenConst::eAssetType _eType )const
{
	zenAssert( _eType < zenConst::keAssType__Count );
	return madAsset[_eType];
}

void ManagerAsset::AssetAdd( const zenAss::zAssetItemRef& _rAsset )
{
	if( _rAsset.IsValid() )
	{
		zenAssert( madAsset[_rAsset->GetType()].Exist(_rAsset->GetID().muIndex) == false );
		madAsset[_rAsset->GetType()].Set(_rAsset->GetIDUInt(), _rAsset);
	}	
}

//! @note Not using a const zAssetItemRef&, to prevent issue if it points to the madAsset entry we're deleting
void ManagerAsset::AssetDelete( zenAss::zAssetItemRef _rAsset )
{
	if( _rAsset.IsValid() )
	{
		_rAsset->SetPackage( zenAss::zPackageRef(nullptr) );
		madAsset[_rAsset.GetType()].Unset(_rAsset.GetIDUInt());
	}
}

bool ManagerAsset::SaveAll()
{
	zxAss::AssetLoaderXml xmlLoader;	
	return mrGroupRoot->Save(xmlLoader, false);
}

} //namespace zeAss
#else

zenPreventEmptyFileWarning

#endif //ZEN_ENGINETOOL
