
#if ZEN_ENGINETOOL
namespace zen { namespace zeAss
{

zenAss::zAssetItem::ID Asset::GetID()const
{
	return mID;
}

const zArrayStatic<zString>& Asset::GetGroupAndName()const
{
	return maGroup;
}

const zString& Asset::GetName()const
{
	return *maGroup.Last();
}

const zString& Asset::GetDescription()const
{
	return mzDescription;
}

zenConst::eAssetType Asset::GetType()const
{
	return mID.meType;
}

template <zenConst::eAssetType TAssetType>
TAsset<TAssetType>::TAsset()
{
	mID.meType = static_cast<zenConst::eAssetType>(kAssetType);
}

//=================================================================================================
//! @brief		
//! @details	
//-------------------------------------------------------------------------------------------------
//! @param hPropertyName	- Name of property to look for
//! @return					- Index of found item (-1 if not found)
//=================================================================================================
template <zenConst::eAssetType TAssetType>
zInt TAsset<TAssetType>::GetValueIndex(zHash32 _hPropertyName)const
{
	static zMap<zInt>::Key32 sdPropertyIndex;
	static bool sbInit(false);
	if( !sbInit )
	{
		const zenAss::PropertyDefArray& aPropertyDef = GetProperties();
		sdPropertyIndex.Init(aPropertyDef.Count()*2);
		sdPropertyIndex.SetDefaultValue(-1);
		for(zInt idx(0), count(aPropertyDef.Count()); idx<count; ++idx)
			sdPropertyIndex.Set( aPropertyDef[idx]->mName.mhName, idx );
		sbInit = true;
	}
	
	return sdPropertyIndex[_hPropertyName];
}


}} //namespace zen { namespace zeAss

#endif
