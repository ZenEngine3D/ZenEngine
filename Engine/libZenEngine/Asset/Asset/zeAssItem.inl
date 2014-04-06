
#if ZEN_ENGINETOOL
namespace zen { namespace zeAss
{



zHash64 Asset::GetID()const
{
	return mhID;
}

const zArrayStatic<zString>& Asset::GetGroupAndName()const
{
	return maGroup;
}

zHash32 Asset::GetGroupID()const
{
	return mhGroupID;
}

const zString& Asset::GetName()const
{
	return *maGroup.Last();
}

const zString& Asset::GetDescription()const
{
	return mzDescription;
}

}} //namespace zen { namespace zeAss

#endif
