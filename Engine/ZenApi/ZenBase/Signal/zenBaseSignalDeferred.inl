namespace zen { namespace zenSig
{

#include <assert.h>

//==================================================================================================
// Utility functions to unpack values from tuple to function parameters
//==================================================================================================
inline void SignalWithPackedParameters(const std::tuple<>& _Params, const std::function<void()>& _Callback)
{
	(void)_Params;
 	_Callback();
}

template<typename TParam1>
inline void SignalWithPackedParameters(const std::tuple<TParam1>& _Params, const std::function<void(TParam1)>& _Callback)
{
	_Callback(std::get<0>(_Params));
}

template<typename TParam1, typename TParam2>
inline void SignalWithPackedParameters(const std::tuple<TParam1,TParam2>& _Params, const std::function<void(TParam1,TParam2)>& _Callback)
{	
	_Callback(std::get<0>(_Params),std::get<1>(_Params));
}

template<typename TParam1, typename TParam2, typename TParam3>
inline void SignalWithPackedParameters(const std::tuple<TParam1,TParam2,TParam3>& _Params, const std::function<void(TParam1,TParam2,TParam3)>& _Callback)
{
	_Callback(std::get<0>(_Params),std::get<1>(_Params),std::get<2>(_Params));
}

template<typename TParam1, typename TParam2, typename TParam3, typename TParam4>
inline void SignalWithPackedParameters(const std::tuple<TParam1,TParam2,TParam3,TParam4>& _Params, const std::function<void(TParam1,TParam2,TParam3,TParam4)>& _Callback)
{
	_Callback(std::get<0>(_Params),std::get<1>(_Params),std::get<2>(_Params),std::get<3>(_Params));
}

template<typename TParam1, typename TParam2, typename TParam3, typename TParam4, typename TParam5>
inline void SignalWithPackedParameters(const std::tuple<TParam1,TParam2,TParam3,TParam4,TParam5>& _Params, const std::function<void(TParam1,TParam2,TParam3,TParam4,TParam5)>& _Callback)
{
	_Callback(std::get<0>(_Params),std::get<1>(_Params),std::get<2>(_Params),std::get<3>(_Params),std::get<4>(_Params));
}

//==================================================================================================
//! @details Keep track of the destruction policy
//==================================================================================================
template<typename... TParameters>
zSignalDeferred<TParameters...>::zSignalDeferred(bool inDispatchOnDestroy)
: mbDispatchOnDestroy(inDispatchOnDestroy)
{
}

//==================================================================================================
//! @details Dispatch or Clear pending signals before destroying this object
//==================================================================================================
template<typename... TParameters>
zSignalDeferred<TParameters...>::~zSignalDeferred()
{
	if( mbDispatchOnDestroy )
		Dispatch();
	else
		Clear();
}

//==================================================================================================
//! @details Append a new signal to this emitter queue
//==================================================================================================
template<typename... TParameters>
void zSignalDeferred<TParameters...>::Clear()
{
	maPendingSignals.clear();
}

//==================================================================================================
//! @details Append a new signal to this emitter queue
//==================================================================================================
template<typename... TParameters>
void zSignalDeferred<TParameters...>::Emit(TParameters...inValues)
{
	// There's too many parameters used in the Emitter template. 
	// Either reduce the amount or add support for more with new SignalWithPackedParameters	
	// This restriction is because haven't found a way to expand saved variadic values to function parameters
	// in C++17, there's the new 'std::apply' that will allow us to pass the tup           le directly as function parameters,
	// thus avoiding those specialized templated function 
	static_assert(sizeof...(TParameters) <= 5, "Unsupported amount of parameters for the signal");
	maPendingSignals.push_back( std::forward_as_tuple(inValues...) );
}

//==================================================================================================
//! @details Broadcast all queued signals to listeners
//==================================================================================================
template<typename... TParameters>
void zSignalDeferred<TParameters...>::Dispatch()
{
	for( const auto& SignalParam : maPendingSignals )
	{
		auto it = mlstSlots.begin();
		while( it != mlstSlots.end() )
		{
			const Slot& slot = *it++; //Increment before invoking callback, so if callback removes slot, won't affect iteration
			SignalWithPackedParameters(SignalParam, slot.GetCallback());			
		}
	}
	maPendingSignals.clear();
}

}} //namespace zen { namespace zenSig