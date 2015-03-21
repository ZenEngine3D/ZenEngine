
#if ZEN_ENGINETOOL

namespace zen { namespace zeAss
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

const zArrayDynamic<zenAss::zPackageRef>& PackageGroup::GetPackages()const
{
	return maPackage;
}

const zArrayDynamic<zenAss::zPackageGroupRef>& PackageGroup::GetChildren()const
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

bool PackageGroup::IsParentOf(const zenAss::zPackageGroupRef& _Child)const
{
	zenAss::zPackageGroupRef rCurrentParent = _Child.IsValid() ? _Child->GetParentGroup() : NULL;
	while( rCurrentParent.IsValid() )
	{
		if( rCurrentParent.Get() == this )
			return true;
		rCurrentParent = rCurrentParent->GetParentGroup();
	}
	return false;
}

bool PackageGroup::IsParentOf(const zenAss::zPackageRef& _Child)const
{
	zenAss::zPackageGroupRef rCurrentParent = _Child.IsValid() ? _Child->GetParentGroup() : NULL;
	while (rCurrentParent.IsValid())
	{
		if (rCurrentParent.Get() == this)
			return true;
		rCurrentParent = rCurrentParent->GetParentGroup();
	}
	return false;
}

}} //namespace zen { namespace zeAss

#endif