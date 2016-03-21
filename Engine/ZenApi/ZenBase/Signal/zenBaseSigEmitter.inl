#pragma once
#ifndef __zenBase_Signal_Emitter_inl__
#define __zenBase_Signal_Emitter_inl__

namespace zen {	namespace zenSig
{


void zSignalEmitter0::Emit()const
{
	ConnectionBase* pConnection = mlstListeners.GetHead();
	while(pConnection)
	{
		static_cast<Connection*>(pConnection)->Emit();
		pConnection = mlstListeners.GetNext(*pConnection);
	}
}

void zSignalEmitter0::DisconnectAll()
{
	Super::DisconnectAll();
}

//#################################################################################################
template<class TParam1>
void zSignalEmitter1<TParam1>::Emit(TParam1 _Param1)const
{
	ConnectionBase* pConnection = mlstListeners.GetHead();
	while (pConnection)
	{
		static_cast<Connection*>(pConnection)->Emit(_Param1);
		pConnection = mlstListeners.GetNext(*pConnection);
	}
}

template<class TParam1>
void zSignalEmitter1<TParam1>::DisconnectAll()
{
	Super::DisconnectAll();
}

//#################################################################################################
template<class TParam1, class TParam2>
void zSignalEmitter2<TParam1, TParam2>::Emit(TParam1 _Param1, TParam2 _Param2)const
{
	ConnectionBase* pConnection = mlstListeners.GetHead();
	while (pConnection)
	{
		static_cast<Connection*>(pConnection)->Emit(_Param1, _Param2);
		pConnection = mlstListeners.GetNext(*pConnection);
	}
}

template<class TParam1, class TParam2>
void zSignalEmitter2<TParam1,TParam2>::DisconnectAll()
{
	Super::DisconnectAll();
}

//#################################################################################################
template<class TParam1, class TParam2, class TParam3>
void zSignalEmitter3<TParam1, TParam2, TParam3>::Emit(TParam1 _Param1, TParam2 _Param2, TParam3 _Param3)const
{
	ConnectionBase* pConnection = mlstListeners.GetHead();
	while (pConnection)
	{
		static_cast<Connection*>(pConnection)->Emit(_Param1, _Param2, _Param3);
		pConnection = mlstListeners.GetNext(*pConnection);
	}
}

template<class TParam1, class TParam2, class TParam3>
void zSignalEmitter3<TParam1,TParam2,TParam3>::DisconnectAll()
{
	Super::DisconnectAll();
}

}	} //namespace zen { namespace zenSig


#endif