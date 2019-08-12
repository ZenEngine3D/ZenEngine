
#if ZEN_ENGINETOOL

namespace zeAss
{
void PackageGroup::SetStorageInfo(const zWString& _zStorageInfo)
{
	mzStorageInfo = _zStorageInfo;
}

const zWString& PackageGroup::GetName()const
{
	return mName;
}

const zenAss::zPackageGroupRef& PackageGroup::GetParentGroup()const
{
	return mrParent;
}

const zArrayDyn<zenAss::zPackageRef>& PackageGroup::GetPackages()const
{
	return maPackage;
}

const zArrayDyn<zenAss::zPackageGroupRef>& PackageGroup::GetChildren()const
{
	return maChildren;
}

zHash64 PackageGroup::GetNameHashID()const
{
	return mNameHashID;
}

const zWString& PackageGroup::GetStorageInfo()const
{
	return mzStorageInfo;
}

bool PackageGroup::GetDirty()const
{
	return mbDirty;
}

bool PackageGroup::GetDirtyChild()const
{
	return mbDirtyChild;
}

} //namespace zeAss

#endif