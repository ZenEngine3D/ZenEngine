#include "libZenEngine.h"

#if ZEN_ENGINETOOL

namespace zen { namespace zeAss
{	
//=================================================================================================
//! @brief		Generates an array with Group names and item name inserted into array
//! @details	Used by Packages and Assets, for their naming hierarchy
//-------------------------------------------------------------------------------------------------
//! @param[in]		_zName		- Item name
//! @param[in]		_zGroup		- Group name as a string (with each element separated by '\' 
//! @param[out]		_aOutGroup	- Array to store result (last element is item name)
//=================================================================================================
void ParseGroupAndName(const char* _zName, const char* _zGroup, zArrayStatic<zString>& _aOutGroup )
{
	if( _zGroup )
	{
		zUInt uGroupEnd[64];
		zUInt uGroupCount(0);
		zUInt pos(0);
		while( _zGroup[pos] != 0 )
		{
			if( _zGroup[pos] == '\\' )
				uGroupEnd[uGroupCount++] = pos;
			++pos;
		}
		if( pos > 0 && _zGroup[pos-1]!= '\\' )
			uGroupEnd[uGroupCount++] = pos;

		pos	= 0;
		_aOutGroup.SetCount(uGroupCount+1);
		for(zUInt idx(0); idx<uGroupCount; ++idx)
		{
			char temp[128];
			zUInt len = zenMath::Min<zUInt>(ZENArrayCount(temp), uGroupEnd[idx]-pos+1 );
			zenMem::Copy(temp, &_zGroup[pos], len);
			temp[len-1]		= 0;
			_aOutGroup[idx]	= temp;
			pos				= uGroupEnd[idx]+1;
		}
	}
	else
	{
		_aOutGroup.SetCount(1);
	}
	*_aOutGroup.Last() = _zName;
}

Package::Package()
: madAssetPerType(zenConst::keAssType__Count)
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
	//Unload();
	for(zUInt typIdx(0), typCount(madAssetPerType.Count()); typIdx<typCount; ++typIdx)
	{
		zMap<zenAss::zAssetItem>::Key64::Iterator it;	
		madAssetPerType[typIdx].GetFirst(it);
		while( it.IsValid() )
		{
			//! @todo Asset: Set package reference to null
			//zenDel(it.GetValue());
			++it;
		}			
		madAssetPerType[typIdx].Clear();
	}
	mFilename = L"";
	maGroup.SetCount(0);
}

void Package::Unload()
{	
	for(zUInt typIdx(0), typCount(madAssetPerType.Count()); typIdx<typCount; ++typIdx)
	{
		zMap<zenAss::zAssetItem>::Key64::Iterator it;	
		madAssetPerType[typIdx].GetFirst(it);
		while( it.IsValid() )
		{
			//! @todo Asset: Set package reference to null
			//zenDel(it.GetValue());
			++it;
		}			
		madAssetPerType[typIdx].Clear();
	}
	mFilename = L"";
	maGroup.SetCount(0);
}

}} //namespace zen { namespace zeAss

#endif //ZEN_ENGINETOOL
