#include "zcCore.h"

namespace zcPerf 
{

zeEventBase::zeEventBase(const zStringHash32& _zEventName)
: mzEventName(_zEventName)
{
}

zeEventBase::~zeEventBase()
{
	zeEventBase* pCounter = mlstChilds.PopHead();
	while( pCounter )
	{
		pCounter->ReferenceRem();
		pCounter = mlstChilds.PopTail();
	}
}

void zeEventBase::AddChild(zeEventBase& _Child)
{
	_Child.ReferenceAdd();
	mlstChilds.PushTail(_Child);
}

}