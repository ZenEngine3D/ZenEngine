#include "zbBase.h"

namespace zen { namespace zenSig
{

	
zSignalEmitterAsync::zSignalEmitterAsync()
: mpEmitterGroup(nullptr)
{
}

zSignalEmitterAsync::zSignalEmitterAsync(class zSignalEmitterAsyncGroup* _pEmitterGroup)
: mpEmitterGroup(_pEmitterGroup)
{
}

zSignalEmitterAsync::~zSignalEmitterAsync()
{
	ClearPendingSignals();
}

void zSignalEmitterAsync::ClearPendingSignals() //!< @Brief Delete all pending signal owned by this emitter
{
	// Signals are stored in 2 lists and need to be cleared from both
	SignalInfo* pSignalCur = mlstPendingSignal.PopTail();
	while (pSignalCur)
	{
		decltype(mlstPendingSignal)::Remove( *pSignalCur );
		pSignalCur = mlstPendingSignal.PopTail();
	}
}

//=================================================================================================
//! @details	Assign owner this emitter belongs to. Order of signals are preserved between 
//!				signals from various emitter belonging to same group.
//! @note		No signal will be sent, if this emitter doesn't have a group assigned.
//-------------------------------------------------------------------------------------------------
//! @param[in]	_pEmitterGroup - Group to assign this Emitter
//=================================================================================================
void zSignalEmitterAsync::SetEmitterGroup(class zSignalEmitterAsyncGroup* _pEmitterGroup)
{
	mpEmitterGroup = _pEmitterGroup;
}

//=================================================================================================
//! @details	
//!				
//-------------------------------------------------------------------------------------------------
//!
//=================================================================================================
zSignalEmitterAsyncGroup::zSignalEmitterAsyncGroup()
{
}

zSignalEmitterAsyncGroup::~zSignalEmitterAsyncGroup()
{
	ClearPendingSignals();
}

void zSignalEmitterAsyncGroup::EmitAllSignals()
{
	zSignalEmitterAsync::SignalInfo* pCurrent = mlstPendingSignal.PopHead();
	while( pCurrent )
	{
		zSignalEmitterAsync::SignalInfo::ListEmitter::Remove(*pCurrent);
		pCurrent->Send();		
		zenDel(pCurrent);
		pCurrent = mlstPendingSignal.PopHead();
	}
}

void zSignalEmitterAsyncGroup::ClearPendingSignals()
{
	// Signals are stored in 2 lists and need to be cleared from both
	zSignalEmitterAsync::SignalInfo* pSignalCur = mlstPendingSignal.PopTail();
	while (pSignalCur)
	{
		zSignalEmitterAsync::SignalInfo::ListEmitter::Remove(*pSignalCur);
		pSignalCur = mlstPendingSignal.PopTail();
	}
}

//=================================================================================================
//! @details	
//!				
//-------------------------------------------------------------------------------------------------
//!
//=================================================================================================
void zSignalEmitterAsync0::Emit()
{
	static zenMem::zAllocatorPool sMemPool("SignalEmitterAsync1 Signal", sizeof(SignalInfo), 256, 256);	
	if( mpEmitterGroup )
	{
		SignalInfo* pPendingSignal	= zenNew(&sMemPool) SignalInfo;
		pPendingSignal->mpEmitter	= this;
		mpEmitterGroup->mlstPendingSignal.PushTail( *pPendingSignal );	
	}
}

void zSignalEmitterAsync0::SignalInfo::Send()
{
	auto it = dynamic_cast<const zSignal*>(mpEmitter)->mlstListeners.GetHeadIt();
	while(it)
	{
		zSignal0::Connection* pConnection = static_cast<zSignal0::Connection*>(it.Get());
		pConnection->Emit();
		++it;
	}
}

} } //namespace zen { namespace zenSig