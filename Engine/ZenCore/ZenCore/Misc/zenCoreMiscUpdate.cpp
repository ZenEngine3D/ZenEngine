#include "zcCore.h"

namespace zen { namespace zenMisc 
{

IUpdate::IUpdate()
: mePriorityGroup(zenConst::keUpdt__Invalid)
, muPriority(0)
{
}

IUpdate::IUpdate(zenConst::eUpdatePriority _ePriorityGroup, zU32 _uPriority)
{
	Activate(_ePriorityGroup, _uPriority);
}

IUpdate::~IUpdate()
{
	Deactivate();
}

void IUpdate::Update()
{
}

void IUpdate::Activate(zenConst::eUpdatePriority _ePriorityGroup, zU32 _uPriority)
{
	ZENAssert(mePriorityGroup<zenConst::keUpdt__Count);
	mePriorityGroup		= _ePriorityGroup;
	muPriority			= _uPriority;
	auto it				= zcMgr::Updater.mlstUpdateable[mePriorityGroup].GetHeadIt();
	while( it->muPriority > _uPriority )
		++it;

	if( it.IsValid() )
		it.InsertBefore(*this);
	else
		zcMgr::Updater.mlstUpdateable[mePriorityGroup].PushTail(*this);
}

void IUpdate::Deactivate()
{
	List::Remove( *this );
}

void IUpdate::Reactivate()
{
	ZENAssertMsg(mePriorityGroup<zenConst::keUpdt__Count, "Make sure object has been activated properly once before re-activating");
	Activate(mePriorityGroup, muPriority);
}

}} //namespace zen { namespace zenMgr {