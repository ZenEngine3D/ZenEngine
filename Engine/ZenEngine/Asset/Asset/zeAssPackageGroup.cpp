#include "zeEngine.h"

#if ZEN_ENGINETOOL

namespace zeAss
{	

PackageGroup::PackageGroup(const zWString& _zName, zenAss::zPackageGroupRef _Parent)
: mName(_zName)
, mrParent(nullptr)
, mNameHashID(_zName)
, mbDirty(false)
, mbDirtyChild(false)
{
	maPackage.Reserve(8);
	maChildren.Reserve(8);
	SetParentGroup(_Parent);	
}

PackageGroup::~PackageGroup()
{
	SetParentGroup( zenAss::zPackageGroupRef(nullptr) );
}

void PackageGroup::SetParentGroup(const zenAss::zPackageGroupRef& _rParent)
{
	if( mrParent != _rParent && _rParent.Get() != this && !IsParentOf(_rParent) )
	{
		if( mrParent.IsValid() )
		{			
			mrParent->maChildren.RemoveSwap(this);
			mrParent->SetDirty();
		}

		mrParent = _rParent;
		if( mrParent.IsValid() )
		{
			mNameHashID = mrParent.GetNameHashID().Append(mName);
			mrParent->maChildren.Push(this);			
		}		

		SetDirty();
	}
}

void PackageGroup::SetName(const zWString& _zName)
{
	if( mName != _zName )
	{
		mName			= _zName;		
		mNameHashID		= mrParent.GetNameHashID().Append(mName);
		SetDirty();
	}
}

void PackageGroup::SetDirty()
{
	if( mbDirty == false && !zeMgr::Asset.IsLoading() )
	{				
		mbDirty = true;
		mrParent->SetDirtyChild();
	}
}

void PackageGroup::SetDirtyChild()
{
	if( !zeMgr::Asset.IsLoading() )
	{
		for (zenAss::zPackageGroupRef rGroup = this; rGroup.IsValid() && !rGroup->mbDirtyChild; rGroup = rGroup.GetParentGroup())
			rGroup->mbDirtyChild = true;
	}
}

zUInt PackageGroup::GetPackagesize() const
{
	zUInt uCount(maPackage.size());
	for(zUInt idx(0),count(maChildren.size()); idx<count; ++idx)
		uCount += maChildren[idx]->GetPackagesize();
	return uCount;
}

bool PackageGroup::Save(AssetSerializer &_Serializer, bool _bDirtyParent )
{
	bool bSuccess(true);	
	if( mbDirty || mbDirtyChild || _bDirtyParent )
	{
		for (zUInt idx(0), count(maChildren.size()); idx < count; ++idx)
			bSuccess &= maChildren[idx]->Save(_Serializer, mbDirty || _bDirtyParent);

		for (zUInt idx(0), count(maPackage.size()); idx < count; ++idx)
			bSuccess &= maPackage[idx]->Save(_Serializer, mbDirty || _bDirtyParent);

		if( mbDirty || _bDirtyParent )
			bSuccess &= _Serializer.Save(zenAss::zPackageGroupRef(this));

		mbDirty			&= !bSuccess;
		mbDirtyChild	&= !bSuccess;
	}
	return bSuccess;
}


void PackageGroup::Delete()
{
	//! @todo Asset move entire function to Asset Manager?
	for (zUInt idx(0), count(maChildren.size()); idx < count; ++idx)
		maChildren[idx]->Delete();

	for (zUInt idx(0), count(maPackage.size()); idx < count; ++idx)
		maPackage[idx]->Delete();

	zenAss::zPackageGroupRef rThis(this);
	maChildren.Clear();
	maPackage.Clear();
	mrParent->maChildren.RemoveSwap( rThis );
	zeMgr::Asset.GroupDelete( rThis );
}

bool PackageGroup::IsParentOf(const zenAss::zPackageGroupRef& _Child)const
{
	zenAss::zPackageGroupRef rCurrentParent = _Child.IsValid() ? _Child->GetParentGroup() : nullptr;
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
	zenAss::zPackageGroupRef rCurrentParent = _Child.IsValid() ? _Child->GetParentGroup() : nullptr;
	while (rCurrentParent.IsValid())
	{
		if (rCurrentParent.Get() == this)
			return true;
		rCurrentParent = rCurrentParent->GetParentGroup();
	}
	return false;
}

} //namespace zeAss

#else

zenPreventEmptyFileWarning

#endif //ZEN_ENGINETOOL
