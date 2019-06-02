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
	zenAssert(mePriorityGroup<zenConst::keUpdt__Count);
	mePriorityGroup			= _ePriorityGroup;
	muPriority				= _uPriority;
	zcMgr::Updater.mlstUpdateable[mePriorityGroup].push_sort(*this);
}

void IUpdate::Deactivate()
{
	List::remove( *this );
}

void IUpdate::Reactivate()
{
	zenAssertMsg(mePriorityGroup<zenConst::keUpdt__Count, "Make sure object has been activated properly once before re-activating");
	Activate(mePriorityGroup, muPriority);
}

}} //namespace zen { namespace zenMgr {