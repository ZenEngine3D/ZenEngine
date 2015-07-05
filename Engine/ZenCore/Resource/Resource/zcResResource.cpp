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

bool Resource::Initialize( )
{
	if( ResourceInit() && zcDepot::Resources.Add(this).IsValid() )
	{
		glstResourceActive[mResID.GetType()].PushHead(*this);
		return true;
	}
	return false;
}

UnsupportedResource::UnsupportedResource()
{
	ZENAssertMsg(0,"Trying to use an unsuported resource type.");
}	

}

