#include "libZenEngine.h"

#if ZEN_ENGINETOOL

namespace zen { namespace zeAss
{

void AssetLoader::AddPackage(zeAss::Package* _pPackage)
{
	zeMgr::Asset.PackageAdd(_pPackage);
}

void AssetLoader::AddAsset(zeAss::Asset* _pAsset)
{
	zeMgr::Asset.AssetAdd(_pAsset);
}

}} //namespace zen { namespace zeAss

#endif //ZEN_ENGINETOOL
