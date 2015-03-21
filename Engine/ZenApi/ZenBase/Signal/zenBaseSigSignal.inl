#pragma once
#ifndef __zenBase_Signal_Signal_inl__
#define __zenBase_Signal_Signal_inl__

namespace zen {	namespace zenSig
{

template<class TSlotObject>
zSignal0::TConnection<TSlotObject>::TConnection(TSlotObject& _SlotObject, Slot _Slot)
: mpObjectMethodSlot(_Slot)
{
}

template<class TSlotObject>
void zSignal0::TConnection<TSlotObject>::Emit()
{
	(static_cast<TSlotObject*>(mpObject)->*mpObjectMethodSlot)();
}

template<class TSlotObject>
void zSignal0::Connect(TSlotObject& _Object, typename TConnection<TSlotObject>::Slot _Slot )
{
	static zenMem::zAllocatorPool sMemPool("zSignal0 Connection", sizeof(TConnection<TSlotObject>), 256, 256);
	TConnection<TSlotObject>* pConnection = zenNew(&sMemPool) TConnection<TSlotObject>(_Object, _Slot);		
	zSignal::Connect(_Object, *pConnection);
}

//#################################################################################################

template<class TParam1> template<class TSlotObject>
zSignal1<TParam1>::TConnection<TSlotObject>::TConnection(TSlotObject& _SlotObject, Slot _Slot )
: mpObjectMethodSlot(_Slot)
{
}

template<class TParam1> template<class TSlotObject>
void zSignal1<TParam1>::TConnection<TSlotObject>::Emit(TParam1 _Param1)
{
	(static_cast<TSlotObject*>(mpObject)->*mpObjectMethodSlot)(_Param1);
}

template<class TParam1> template<class TSlotObject>
void zSignal1<TParam1>::Connect(TSlotObject& _Object, typename TConnection<TSlotObject>::Slot _Slot)
{
	static zenMem::zAllocatorPool sMemPool("zSignal1 Connection", sizeof(TConnection<TSlotObject>), 256, 256);
	TConnection<TSlotObject>* pConnection = zenNew(&sMemPool) TConnection<TSlotObject>(_Object, _Slot);
	zSignal::Connect(_Object, *pConnection);
}

//#################################################################################################

template<class TParam1, class TParam2> template<class TSlotObject>
zSignal2<TParam1, TParam2>::TConnection<TSlotObject>::TConnection(TSlotObject& _SlotObject, Slot _Slot)
: mpObjectMethodSlot(_Slot)
{
}

template<class TParam1, class TParam2> template<class TSlotObject>
void zSignal2<TParam1, TParam2>::TConnection<TSlotObject>::Emit(TParam1 _Param1, TParam2 _Param2)
{
	(static_cast<TSlotObject*>(mpObject)->*mpObjectMethodSlot)(_Param1, _Param2);
}

template<class TParam1, class TParam2> template<class TSlotObject>
void zSignal2<TParam1, TParam2>::Connect(TSlotObject& _Object, typename TConnection<TSlotObject>::Slot _Slot)
{
	static zenMem::zAllocatorPool sMemPool("zSignal2 Connection", sizeof(TConnection<TSlotObject>), 256, 256);
	TConnection<TSlotObject>* pConnection = zenNew(&sMemPool) TConnection<TSlotObject>(_Object, _Slot);
	zSignal::Connect(_Object, *pConnection);
}

//#################################################################################################

template<class TParam1, class TParam2, class TParam3> template<class TSlotObject>
zSignal3<TParam1, TParam2, TParam3>::TConnection<TSlotObject>::TConnection(TSlotObject& _SlotObject, Slot _Slot)
: mpObjectMethodSlot(_Slot)
{
}

template<class TParam1, class TParam2, class TParam3> template<class TSlotObject>
void zSignal3<TParam1, TParam2, TParam3>::TConnection<TSlotObject>::Emit(TParam1 _Param1, TParam2 _Param2, TParam3 _Param3)
{
	(static_cast<TSlotObject*>(mpObject)->*mpObjectMethodSlot)(_Param1, _Param2, _Param3);
}

template<class TParam1, class TParam2, class TParam3> template<class TSlotObject>
void zSignal3<TParam1, TParam2, TParam3>::Connect(TSlotObject& _Object, typename TConnection<TSlotObject>::Slot _Slot)
{
	static zenMem::zAllocatorPool sMemPool("zSignal3 Connection", sizeof(TConnection<TSlotObject>), 256, 256);
	TConnection<TSlotObject>* pConnection = zenNew(&sMemPool) TConnection<TSlotObject>(_Object, _Slot);
	zSignal::Connect(_Object, *pConnection);
}

}	} //namespace zen { namespace zenSig


#endif