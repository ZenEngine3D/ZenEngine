#pragma once

#include <functional>
#include <EASTL/intrusive_list.h>

namespace zen { namespace zenSig
{

//==================================================================================================
//! @Class		Signal/Slot systems for any type of callbacks
//! @details	Can can be used to implement events/listeners system. It is simple to use and fast
//!				but without multi-threading support implemented. 
//!				A derived class can extend support for more complex behaviors, like async messages.
//!				In Signal declaration, Callback function parameter(s) is defined.
//==================================================================================================
template<typename... TParameters>
class zSignal
{
public:
	//----------------------------------------------------------------------------------------------
	//! @Class	Class connecting a Callback to a Signal
	//! @detail Use intrusive list for Signal to keep tab of all listeners. Intrusive list means 
	//!			that we avoid slow dynamic allocation and we can easily remove this slot from the
	//!			list when its owner goes out of scope, making sure we don't try dispatching to 
	//!			deleted objects/callback.
	//!			Alternatively, we could use a dynamic table of slots, to have a better 
	//!			cache coherency, with a pointer to owner signal to make sure Slot is removed from it
	//!			when destroyed, just like with the intrusive list.
	//----------------------------------------------------------------------------------------------
	class Slot : public eastl::intrusive_list_node
	{	
	public:									
		using Callback	= std::function<void(TParameters...)>;										//!< Callback function signature
		using Signal	= zSignal<TParameters...>;													//!< Useful to get Signal type associated to this Slot type

	public:
								Slot();
								~Slot();															//!< Remove this slot from list kept in Signal, when Slot is destroyed, preventing problem with destroyed objects
		inline void				Connect(zSignal& inSignal,const typename Callback& inCallback);		//!< Bind Signal to a Callback using a Slot to store infos
		inline void				Disconnect();														//!< Remove this Slot from Signal listeners list
		inline const Callback&	GetCallback()const;													//!< Let Signal retrieve the callback and invoke it
	protected:
		Callback				mCallback;															//!< Functions emitter should call when signaling
	};

	//----------------------------------------------------------------------------------------------
	// Main content of the class
	//----------------------------------------------------------------------------------------------
public:	
	inline void					Emit(TParameters...)const;											//!< Signal all Slots connected to this Signal
	inline void					DisconnectAll();													//!< Remove all Slots connected to this Signal

protected:
	eastl::intrusive_list<Slot>	mlstSlots;															//!< Linked list of all Slots waiting for Signals	
	friend class Slot;																				//!< Allow access to mlstSlots when calling 'Connect'	
};

}} //namespace zen { namespace zenSig

#include "zenBaseSignal.inl"
