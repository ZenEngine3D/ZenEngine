#include "libZenEngine.h"

#if ZEN_ENGINETOOL

namespace zen { namespace zenAss
{

const zPackageRef& zPackageRef::sGetPackage( zU32 _hPackageID )
{
	return zeMgr::Asset.PackageGet(_hPackageID);
}

const zArrayPackage& zPackageRef::sGetPackages()
{
	return zeMgr::Asset.PackageGet();
}

zPackageRef zPackageRef::sCreate(const zWString& _zName, const zPackageGroupRef& _rParent)
{
	if( _rParent.IsValid() )
	{
		zenAss::zPackageRef rNewPackage = zenNewDefault zeAss::Package;
		if( rNewPackage->Init(zeMgr::Asset.GetPackageNextID(), _zName, _rParent, zenConst::keEngineVersion__Current) )
		{
			zeMgr::Asset.PackageAdd(rNewPackage);
			return rNewPackage;
		}
	}
	return NULL;
}

zPackageRef::zPackageRef()
:Super()
{
}

zPackageRef::zPackageRef(const zPackageRef& _Copy)
: Super(_Copy)
{
}

zPackageRef::zPackageRef(zeAss::Package* _pPackage)
: Super(_pPackage)
{
}

zU32 zPackageRef::GetID()const									
{ 
	return GetSafe()->GetID(); 
}

const zWString& zPackageRef::GetName()const							
{ 
	return GetSafe()->GetName(); 
}

void zPackageRef::SetParentGroup(const zPackageGroupRef& _rParent)
{
	GetSafe()->SetParentGroup(_rParent);
}

void zPackageRef::SetName(const zWString& _zName)
{
	return GetSafe()->SetName(_zName);
}

void zPackageRef::SetDirty()
{
 	GetSafe()->SetDirty();
}


bool zPackageRef::GetDirty()const
{
	return GetSafe()->GetDirty();
}

const zArrayAsset& zPackageRef::GetAssets(zenConst::eAssetType _eType) const
{
	return GetSafe()->GetAsset(_eType); 
}

const zPackageGroupRef& zPackageRef::GetParentGroup()const
{
	return GetSafe()->GetParentGroup();
}

void zPackageRef::Delete()
{
	GetSafe()->Delete();
}

}} //namespace zen { namespace zenAss

#endif //ZEN_ENGINETOOL
