
#if ZEN_ENGINETOOL
namespace zen { namespace zeAss
{

zU32 Asset::GetID()const
{
	return muID;
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

}} //namespace zen { namespace zeAss

#endif
