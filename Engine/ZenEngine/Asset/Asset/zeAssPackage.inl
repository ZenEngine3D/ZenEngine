#if ZEN_ENGINETOOL
namespace zeAss
{

zU32 Package::GetID()const
{
	return mID;
}

const zWString& Package::GetName()const
{
	return mzName;
}

const zenAss::zPackageGroupRef&	Package::GetParentGroup()const
{
	return mrGroupParent;
}

const zWString& Package::GetStorageInfo()const
{
	return mzStorageInfo;
}

const zenAss::zAssetItemRef& Package::GetAsset(zenConst::eAssetType _eType, zU32 _uAssetID)const
{
	return madAssetPerType[_eType][_uAssetID];
}

const zenAss::zArrayAsset& Package::GetAsset(zenConst::eAssetType _eType)const
{
	return madAssetPerType[_eType];
}

void Package::AssetAdd(const zenAss::zAssetItemRef& _rAsset)
{
	zenAssert( madAssetPerType[_rAsset.GetType()].Exist(_rAsset.GetIDUInt()) == false );
	madAssetPerType[_rAsset.GetType()].Set( _rAsset.GetIDUInt(), _rAsset);
}

void Package::AssetRem(const zenAss::zAssetItemRef& _rAsset)
{
	madAssetPerType[_rAsset.GetType()].Unset( _rAsset.GetIDUInt() );
}

bool Package::GetDirty()const
{
	return mbDirty;
}

} //namespace zeAss

#endif
