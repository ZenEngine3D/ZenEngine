#include "zeEngine.h"

#if ZEN_ENGINETOOL

namespace zeAss
{

void AssetSerializer::AddPackage(const zenAss::zPackageRef& _rPackage)
{	
	zeMgr::Asset.PackageAdd(_rPackage);
}

void AssetSerializer::AddAsset(const zenAss::zAssetItemRef& _rAsset)
{
	zeMgr::Asset.AssetAdd(_rAsset);
}

} //namespace zeAss

#endif //ZEN_ENGINETOOL
