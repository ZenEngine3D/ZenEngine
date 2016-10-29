#include "zeEngine.h"

#if ZEN_ENGINETOOL

namespace zen { namespace zenAss
{
	
const zPackageGroupRef&	zPackageGroupRef::sGetRoot()
{
	return zeMgr::Asset.GroupGetRoot();
}

zPackageGroupRef zPackageGroupRef::sCreate(const zWString& _zName, const zPackageGroupRef& _rParent)
{
	if( _rParent.IsValid() )
		return zeMgr::Asset.GroupCreate(_zName, _rParent);
	else
		return zeMgr::Asset.GroupCreate(_zName, sGetRoot());

}

bool SaveAll()
{
	return zeMgr::Asset.SaveAll();
}

zPackageGroupRef::zPackageGroupRef()
:Super()
{
}

zPackageGroupRef::zPackageGroupRef(const zPackageGroupRef& _Copy)
: Super(_Copy)
{
}

zPackageGroupRef::zPackageGroupRef(zeAss::PackageGroup* _pGroup)
: Super(_pGroup)
{
}

const zWString& zPackageGroupRef::GetName()const
{
	return GetSafe()->GetName();
}

zHash64 zPackageGroupRef::GetNameHashID()const
{
	return GetSafe()->GetNameHashID(); 
}

const zPackageGroupRef& zPackageGroupRef::GetParentGroup()const
{
	return GetSafe()->GetParentGroup();
}

const zArrayDynamic<zenAss::zPackageRef>& zPackageGroupRef::GetPackages()const
{
	return GetSafe()->GetPackages();
}

const zArrayDynamic<zPackageGroupRef>& zPackageGroupRef::GetChildren()const
{
	return GetSafe()->GetChildren();
}

zUInt zPackageGroupRef::GetPackageCount()const
{
	return GetSafe()->GetPackageCount();
}

bool zPackageGroupRef::GetDirty()const
{
	return GetSafe()->GetDirty();
}

bool zPackageGroupRef::GetDirtyChild()const
{
	return GetSafe()->GetDirtyChild();
}

void zPackageGroupRef::SetParentGroup(const zPackageGroupRef& _rParent)
{
	GetSafe()->SetParentGroup(_rParent);
}

void zPackageGroupRef::SetName(const zWString& _zName)
{
	GetSafe()->SetName(_zName);
}

void zPackageGroupRef::Delete()
{
	GetSafe()->Delete();
}

bool zPackageGroupRef::IsParentOf(const zenAss::zPackageGroupRef& _Child)const
{
	return GetSafe()->IsParentOf(_Child);
}

bool zPackageGroupRef::IsParentOf(const zenAss::zPackageRef& _Child)const
{
	return GetSafe()->IsParentOf(_Child);
}

/*
void zPackageGroupRef::Save()
{
	GetSafe()->Save();
}
*/
}} //namespace zen { namespace zenAss
#else

zenPreventEmptyFileWarning

#endif //ZEN_ENGINETOOL
