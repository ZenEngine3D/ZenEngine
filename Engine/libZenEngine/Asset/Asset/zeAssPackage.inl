#if ZEN_ENGINETOOL
namespace zen { namespace zeAss
{

zHash64 Package::GetID()const
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

const zenAss::zAssetItem& Package::GetAsset(zenConst::eAssetType _eType, zHash64 _hAssetName)
{
	return madAssetPerType[_eType][_hAssetName];
}

const zMap<zenAss::zAssetItem>::Key64& Package::GetAsset(zenConst::eAssetType _eType)
{
	return madAssetPerType[_eType];
}

void Package::AssetAdd(zeAss::AssetItem& _Asset)
{
	ZENAssert( madAssetPerType[_Asset.GetType()].Exist(_Asset.GetID()) == false );
	madAssetPerType[_Asset.GetType()].Set( _Asset.GetID(), &_Asset);
}

void Package::AssetRem(zeAss::AssetItem& _Asset)
{
	madAssetPerType[_Asset.GetType()].Unset( _Asset.GetID() );
}

//=================================================================================================
//! @brief		Initialize a Package
//! @details	Receive all needed infos to load a Package.
//-------------------------------------------------------------------------------------------------
//! @param		_zGroup		- String (separated by '\') giving the group hierarchy to put package under
//! @param		_Filename	- Filename of the Package
//=================================================================================================
void Package::Init(const char* _zGroup, const CFil::Filename& _Filename )
{
	char zName[128];

	wcstombs(zName,_Filename.GetNameNoExt(),sizeof(zName));	//! @todo Optim: needed?
	ParseGroupAndName(zName, _zGroup, maGroup );

	mID			= zHash32(_Filename.GetNameFull());
	mhGroupID	= zHash32("Package");
	mFilename	= _Filename;
	for(int idx(0), count(maGroup.Count()-1); idx<count; ++idx )
		mhGroupID.Append( maGroup[idx] );
}

}} //namespace zen { namespace zeAss

#endif
