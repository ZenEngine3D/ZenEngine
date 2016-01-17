#pragma once
#ifndef __zenBase_Signal_Emitter_inl__
#define __zenBase_Signal_Emitter_inl__

namespace zen {	namespace zenSig
{


void zSignalEmitter0::Emit()const
{
	auto it = mlstListeners.GetHeadIt();
	while( *it )
	{
		Connection* pConnection = static_cast<Connection*>(*it);
		pConnection->Emit();
		++it;
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
	auto it = mlstListeners.GetHeadIt();
	while (*it)
	{
		Connection* pConnection = static_cast<Connection*>(*it);
		pConnection->Emit(_Param1);
		++it;
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
	auto it = mlstListeners.GetHeadIt();
	while (*it)
	{
		Connection* pConnection = static_cast<Connection*>(*it);
		pConnection->Emit(_Param1, _Param2);
		++it;
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
	auto it = mlstListeners.GetHeadIt();
	while (*it)
	{
		Connection* pConnection = static_cast<Connection*>(*it);
		pConnection->Emit(_Param1, _Param2, _Param3);
		++it;
	}
}

template<class TParam1, class TParam2, class TParam3>
void zSignalEmitter3<TParam1,TParam2,TParam3>::DisconnectAll()
{
	Super::DisconnectAll();
}

}	} //namespace zen { namespace zenSig


#endif