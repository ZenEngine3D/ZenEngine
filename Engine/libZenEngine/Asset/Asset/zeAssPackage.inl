#if ZEN_ENGINETOOL
namespace zen { namespace zeAss
{

zU32 Package::GetID()const
{
	return mID;
}

const zArrayStatic<zString>& Package::GetGroupAndName()const
{
	return maGroup;
}

const zString& Package::GetName()const
{
	return *maGroup.Last();
}

const zWString& Package::GetStorageName()const
{
	return mzStorageName;
}

const zenAss::zAssetItem& Package::GetAsset(zenConst::eAssetType _eType, zU32 _uAssetID)
{
	return madAssetPerType[_eType][_uAssetID];
}

const zenAss::zArrayAsset& Package::GetAsset(zenConst::eAssetType _eType)
{
	return madAssetPerType[_eType];
}

void Package::AssetAdd(zeAss::Asset& _Asset)
{
	ZENAssert( madAssetPerType[_Asset.GetType()].Exist(_Asset.GetID().muIndex) == false );
	madAssetPerType[_Asset.GetType()].Set( _Asset.GetID().muIndex, &_Asset);
}

void Package::AssetRem(zeAss::Asset& _Asset)
{
	madAssetPerType[_Asset.GetType()].Unset( _Asset.GetID().muIndex );
}

void Package::SetDirty(bool _bDirty)
{
	mbIsDirty = _bDirty;
}

bool Package::IsDirty()const
{
	return mbIsDirty;
}



}} //namespace zen { namespace zeAss

#endif
