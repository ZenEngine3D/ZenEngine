#include "zcCore.h"

namespace zcPerf 
{

EventBase::EventBase(const zStringHash32& _zEventName)
: mzEventName(_zEventName)
{
}

EventBase::~EventBase()
{
	EventBase* pCounter = mlstChilds.pop_front();
	while( pCounter )
		pCounter = mlstChilds.pop_back();
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
	mlstChilds.push_back(_Child);
}

}