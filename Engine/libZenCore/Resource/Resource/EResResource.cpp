#include "libZenCore.h"

namespace ERes
{	

//! @todo Missing: keep track of this
zList2x Resource::spResourceActive[zenConst::keResType__Count];
zList2x Resource::spActiveToDelete[3];
zUInt Resource::suDeleteIndex(0);

//=================================================================================
Resource::Resource()
{
	//spActiveToDelete[suDeleteIndex].AddHead(this);	//Mark to delete, until created properly
}

Resource::~Resource()
{
	//LstRemove();
}

bool Resource::ResourceInit()		
{ 
	return true; 
}

bool Resource::ResourceCreate( EExp::ExportItem& _SerialItem, EExp::ExportInfoBase& _ExportInfo )
{
	// Export the new resource SerialItem
	_SerialItem.Export( _ExportInfo );

	// Create the new resource
	if( _ExportInfo.IsSuccess() && _SerialItem.mResID.IsValid() )
	{
		mResID = _SerialItem.mResID;
		if( ResourceInit() && EMgr::Resources.Add(this).IsValid() )
		{
			//spResourceActive[mResID.Type()].AddHead(this);
			return true;
		}
	}
	return false;
}

//! @todo Missing: Plan this better. Keep list of live resources
bool Resource::ResourceCreateRuntime()
{
	if( EMgr::Resources.Add(this).IsValid() )
	{
		//spResourceActive[mResID.Type()].AddHead(this);
		return true;
	}
	return false;
}

UnsupportedResource::UnsupportedResource()
{
	ZENAssertMsg(0,"Trying to use an unsuported resource type.");
}	

}

