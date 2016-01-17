#include "zcCore.h"

namespace zcRes
{	

//! @todo Missing: keep track of this
static Resource::TypeList glstResourceActive[zenConst::keResType__Count];
static Resource::TypeList glstActiveToDelete[3];
static zUInt guDeleteIndex(0);

//=================================================================================
Resource::Resource()
{
}

Resource::~Resource()
{
	zcDepot::ResourceData.ClearItem(mResID);
	zcDepot::Resources.Remove(mResID);
}

bool Resource::ResourceInit()		
{ 
	return true; 
}

bool Resource::Initialize( )
{
	if( ResourceInit() )
	{
		zcDepot::Resources.Add(this);
		return true;
	}
	return false;
}

UnsupportedResource::UnsupportedResource()
{
	ZENAssertMsg(0,"Trying to use an unsuported resource type.");
}	

}

