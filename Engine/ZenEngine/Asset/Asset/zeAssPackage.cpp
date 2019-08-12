#include "zeEngine.h"

#if ZEN_ENGINETOOL

namespace zeAss
{	

Package::Package()
: madAssetPerType(zenConst::keAssType__Count)
, mzStorageInfo(zenT(""))
, mbDirty(false)
{
	for(zUInt typIdx(0), typCount(madAssetPerType.size()); typIdx<typCount; ++typIdx)
	{
		madAssetPerType[typIdx].Init(50);
		madAssetPerType[typIdx].SetDefaultValue(nullptr);
		madAssetPerType[typIdx].SetDeleteItemCB(ResetAssetReference);
	}
}

Package::~Package()
{
	Unload();	
}

void Package::Unload()
{	
	for(zUInt typIdx(0), typCount(madAssetPerType.size()); typIdx<typCount; ++typIdx)
	{
		while( madAssetPerType[typIdx].size() )
		{
			zUInt debug = madAssetPerType[typIdx].size();
			zenAss::zArrayAsset::Iterator it;
			madAssetPerType[typIdx].GetFirst(it);
			zeMgr::Asset.AssetDelete(it.GetValue());
		}
	}
	mzStorageInfo	= zenT("");
	mzName			= zenT("");
	SetParentGroup(zenAss::zPackageGroupRef(nullptr));
}

void Package::SetName(const zWString& _zName)
{
	if( _zName != mzName )
	{		
		mzName		= _zName;
		SetDirty();
	}	
}

void Package::SetStorageInfo(const zWString& _zStorageInfo)
{
	mzStorageInfo = _zStorageInfo;
}

void Package::SetDirty()
{
	if( !zeMgr::Asset.IsLoading() )
	{
		mbDirty = true;
		if( mrGroupParent.IsValid() )
			mrGroupParent->SetDirtyChild();
	}
}

//=================================================================================================
//! @brief		Initialize a Package
//! @details	Receive all needed infos to use a Package.
//-------------------------------------------------------------------------------------------------
//! @param _zName			- Package name
//! @param _rParentGroup	- Group to put package under
//! @param _zStorageName	- Source of the package	
//=================================================================================================
bool Package::Init(zU32 _uID, const zWString& _zName, const zenAss::zPackageGroupRef& _rParentGroup, zU32 _uEngineVersion)
{
	mID				= _uID != 0 ? _uID :  zeMgr::Asset.GetPackageNextID(); 
	mzName			= _zName;	
	muEngineVersion	= _uEngineVersion;
	SetParentGroup( _rParentGroup );		
	return true;
}

void Package::SetParentGroup(const zenAss::zPackageGroupRef& _rParent)
{
	if( mrGroupParent != _rParent )
	{
		if( mrGroupParent.IsValid() )
			mrGroupParent->maPackage.RemoveSwap(this);

		mrGroupParent	= _rParent;
		if( mrGroupParent.IsValid() )
			mrGroupParent->maPackage.Push(this);

		SetDirty();
	}
}

bool Package::Save(AssetSerializer &_Serializer, bool _bDirtyParent)
{
	bool bSuccess(true);
	if( mbDirty || _bDirtyParent )
		bSuccess = _Serializer.Save( zenAss::zPackageRef(this) );

	mbDirty &= !bSuccess;
	return bSuccess;
}

void Package::Delete()
{	
	//! @todo Asset move entire function to Asset Manager?
	zeMgr::Asset.PackageDelete( zenAss::zPackageRef(this) );
}

} //namespace zeAss
#else

zenPreventEmptyFileWarning

#endif //ZEN_ENGINETOOL
