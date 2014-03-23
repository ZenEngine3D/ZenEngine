
#if ZEN_ENGINETOOL
namespace zen { namespace zeAss
{



zHash64 AssetItem::GetID()const
{
	return mhID;
}

const zArrayStatic<zString>& AssetItem::GetGroupAndName()const
{
	return maGroup;
}

zHash32 AssetItem::GetGroupID()const
{
	return mhGroupID;
}

const zString& AssetItem::GetName()const
{
	return *maGroup.Last();
}

const zString& AssetItem::GetDescription()const
{
	return mzDescription;
}

}} //namespace zen { namespace zeAss

#endif
