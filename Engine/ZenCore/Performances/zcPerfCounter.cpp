#include "zcCore.h"

namespace zcPerf 
{

EventBase::EventBase(const zStringHash32& _zEventName)
: mzEventName(_zEventName)
{
}

EventBase::~EventBase()
{
	EventBase* pCounter = mlstChilds.PopHead();
	while( pCounter )
	{
		pCounter->ReferenceRem();
		pCounter = mlstChilds.PopTail();
	}
}

bool EventBase::IsActive() const
{
	return mbActive;
}

double EventBase::GetElapsedMs()
{ 
	return muTimeElapsed/1000.0; 
}

void EventBase::AddChild(EventBase& _Child)
{
	_Child.ReferenceAdd();
	mlstChilds.PushTail(_Child);
}


}