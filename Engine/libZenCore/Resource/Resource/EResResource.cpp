#include "libZenCore.h"

namespace ERes
{	

//! @todo keep track of this
zenList2x Resource::spResourceActive[awconst::keResType__Count];
zenList2x Resource::spActiveToDelete[3];
zenUInt Resource::suDeleteIndex(0);

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

//! @todo : plan this better
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

