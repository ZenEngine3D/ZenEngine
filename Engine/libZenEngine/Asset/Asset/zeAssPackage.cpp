#include "libZenEngine.h"

#if ZEN_ENGINETOOL

namespace zen { namespace zeAss
{	

Package::Package()
: madAssetPerType(zenConst::keAssType__Count)
, mzStorageName(zenT(""))
, mbIsDirty(false)
{
	for(zUInt typIdx(0), typCount(madAssetPerType.Count()); typIdx<typCount; ++typIdx)
	{
		madAssetPerType[typIdx].Init(50);
		madAssetPerType[typIdx].SetDefaultValue(NULL);
		madAssetPerType[typIdx].SetDeleteItemCB(ResetAssetReference);
	}
}

Package::~Package()
{
	Unload();	
}

void Package::Unload()
{	
	for(zUInt typIdx(0), typCount(madAssetPerType.Count()); typIdx<typCount; ++typIdx)
	{
		zenAss::zArrayAsset::Iterator it(madAssetPerType[typIdx]);	
		while( it.IsValid() )
		{
			//! @todo Asset: Set package reference to null
			//zenDel(it.GetValue());
			++it;
		}			
		madAssetPerType[typIdx].Clear();
	}
	mzStorageName = zenT("");
	maGroup.SetCount(0);
}

//=================================================================================================
//! @brief		Initialize a Package
//! @details	Receive all needed infos to use a Package.
//-------------------------------------------------------------------------------------------------
//! @param _zName			- Package name
//! @param _zGroup			- Group to put package under (separated by '\')
//! @param _zStorageName	- Source of the package	
//=================================================================================================
bool Package::Init(zU32 _uID, const char* _zName, const char* _zGroup, const zChar* _zStorageName, zU32 _uEngineVersion)
{
	zString::Split(_zGroup, '\\', maGroup, 1);
	mID				= _uID != 0 ? _uID :  zeMgr::Asset.GetPackageNextID(); 
	*maGroup.Last() = _zName;
	mzStorageName	= _zStorageName;	
	muEngineVersion	= _uEngineVersion;
	return true;
}

bool Package::Save()
{
	return zeMgr::Asset.PackageSave( mID );
}

}} //namespace zen { namespace zeAss

#endif //ZEN_ENGINETOOL
