#include "libZenEngine.h"

#if ZEN_ENGINETOOL

namespace zen { namespace zenAss
{

const zPackage& GetPackage( zHash64 _hPackageID )
{
	return zeMgr::Asset.PackageGet(_hPackageID);
}

const zArrayPackage& GetPackages()
{
	return zeMgr::Asset.PackageGet();
}

zPackage::zPackage()
:Super()
{
}

zPackage::zPackage(const zPackage& _Copy)
: Super(_Copy)
{
}

zPackage::zPackage(zeAss::Package* _pAsset)
: Super(_pAsset)
{
}

zPackage& zPackage::operator=(const zPackage& _Copy)
{
	Super::operator=(_Copy);
	return *this;
}

zU32 zPackage::GetID()const									
{ 
	ZENAssert(IsValid());
	return zEngineRefConst::Get()->GetID(); 
}
const zString& zPackage::GetName()const							
{ 
	ZENAssert(IsValid());
	return zEngineRefConst::Get()->GetName(); 
}
const zArrayStatic<zString>& zPackage::GetGroupAndName()const	
{ 
	ZENAssert(IsValid());
	return zEngineRefConst::Get()->GetGroupAndName(); 
}
const zArrayAsset& zPackage::GetAssets(zenConst::eAssetType _eType)
{
	ZENAssert(IsValid());
	return Get()->GetAsset(_eType); 
}

}} //namespace zen { namespace zenAss

#endif //ZEN_ENGINETOOL
