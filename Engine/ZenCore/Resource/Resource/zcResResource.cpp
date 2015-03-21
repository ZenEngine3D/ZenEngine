#include "zcCore.h"

namespace zcRes
{	

//! @todo Missing: keep track of this
static zListDeclare(Resource, mlnkList) glstResourceActive[zenConst::keResType__Count];
static zListDeclare(Resource, mlnkList) glstActiveToDelete[3];
static zUInt guDeleteIndex(0);

//=================================================================================
Resource::Resource()
{
	//spActiveToDelete[suDeleteIndex].AddHead(this);	//Mark to delete, until created properly
}

Resource::~Resource()
{
}

bool Resource::ResourceInit()		
{ 
	return true; 
}

bool Resource::ResourceCreate( zcExp::ExportItem& _SerialItem, zcExp::ExportInfoBase& _ExportInfo )
{
	// Export the new resource SerialItem
	_SerialItem.Export( _ExportInfo );

	// Create the new resource
	if( _ExportInfo.IsSuccess() && _SerialItem.mResID.IsValid() )
	{
		mResID = _SerialItem.mResID;
		if( ResourceInit() && EMgr::Resources.Add(this).IsValid() )
		{
			glstResourceActive[mResID.Type()].PushHead(*this);
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
		glstResourceActive[mResID.Type()].PushHead(*this);
		return true;
	}
	return false;
}

UnsupportedResource::UnsupportedResource()
{
	ZENAssertMsg(0,"Trying to use an unsuported resource type.");
}	

}

