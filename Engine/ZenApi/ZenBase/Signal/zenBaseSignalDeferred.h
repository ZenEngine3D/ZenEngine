#pragma once

#include <EASTL/vector.h>

namespace zen { namespace zenSig
{

//==================================================================================================
//! @Class Signal/Slot system for any type of callbacks that needs to be send asynchronously
//! @details	Like 'zSignal', used for Signal/Slot system, but doesn't broadcast immediately,
//!				queue signals until SignalDispatch() is called.
//! @todo 1	Perf See about best parameters container instead of relying on eastl::vector
//==================================================================================================
template<typename... TParameters>
class zSignalDeferred : public zSignal<TParameters...>
{
public:	
				zSignalDeferred(bool inDispatchOnDestroy=false);
				~zSignalDeferred();
	inline void Emit(TParameters...inValues);							//!< Add a signal to this signaler queue 
	inline void Dispatch();												//!< Send out all queued signals in this emitter to all listeners
	inline void Clear();												//!< Clear queued signals without dispatching them
protected:
	bool										mbDispatchOnDestroy;	//!< if we should clear or dispatch pending signals when emitter is destroyed
	eastl::vector<std::tuple<TParameters...>>	maPendingSignals;		//!< Save queued signals parameters until we dispatch them
};

}} //namespace zen { namespace zenSig

#include "zenBaseSignalDeferred.inl"
