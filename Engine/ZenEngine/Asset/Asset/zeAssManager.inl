#if ZEN_ENGINETOOL
namespace zeAss
{	

const zenAss::zPackageGroupRef& ManagerAsset::GroupGetRoot()const
{
	return mrGroupRoot;
}

const zenAss::zArrayPackage& ManagerAsset::PackageGet()const
{
	return mdPackage;
}

zenAss::zAssetID ManagerAsset::GetAssetNextID(zenConst::eAssetType _eType)
{ 
	ZENAssert(_eType < zenConst::keAssType__Count); 
	return zenAss::zAssetID(_eType, maAssetNextID[_eType]++);
}

zU32 ManagerAsset::GetPackageNextID()
{
	return muPackageNextID++;
}

bool ManagerAsset::IsLoading()
{
	return mbLoading;
}

} //namespace zeAss


#endif