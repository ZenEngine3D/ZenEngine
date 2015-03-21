
#if ZEN_ENGINETOOL
namespace zen { namespace zeAss
{

const zenAss::zAssetID& Asset::GetID()const
{
	return mID;
}

zU64 Asset::GetIDUInt()const
{
	return mID.ToUInt();
}

const zString& Asset::GetName()const
{
	return mzName;
}

const zString& Asset::GetDescription()const
{
	return mzDescription;
}

zenConst::eAssetType Asset::GetType()const
{
	return mID.meType;
}

zUInt Asset::GetValueCount()const 
{ 
	return maPropertyValue.Count(); 
}

zenAss::PropertyValueRef Asset::GetValue(zUInt _uValIndex)
{ 
	ZENAssert(_uValIndex < maPropertyValue.Count()); 
	return maPropertyValue[_uValIndex]; 
}

zenAss::PropertyValueRef Asset::GetValue(zHash32 _hPropertyName)
{
	zInt iIndex = GetValueIndex(_hPropertyName);
	if( iIndex >= 0 )
		return GetValue(iIndex);
	return zenAss::PropertyValueRef();
}

Asset::SignalUpdatedProperty& Asset::GetSignalUpdatedProperty()
{
	return msigUpdatedProperty;
}

Asset::SignalUpdatedProperties& Asset::GetSignalUpdatedProperties()
{
	return msigUpdatedProperties;
}

template <zenConst::eAssetType AssetTypedType>
AssetTyped<AssetTypedType>::AssetTyped()
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
template <zenConst::eAssetType AssetTypedType>
zInt AssetTyped<AssetTypedType>::GetValueIndex(zHash32 _hPropertyName)const
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
