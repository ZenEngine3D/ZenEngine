#pragma once
#ifndef __zenBase_Signal_EmitterAsync_inl__
#define __zenBase_Signal_EmitterAsync_inl__

namespace zen {	namespace zenSig
{

//#################################################################################################
template<class TParam1>
zenMem::zAllocatorFastPool< typename zSignalEmitterAsync1<TParam1>::SignalInfo, true, false >& zSignalEmitterAsync1<TParam1>::GetPool()
{
	static zenMem::zAllocatorFastPool< zSignalEmitterAsync1<TParam1>::SignalInfo, true, false> sMemPool(256,256);
	return sMemPool;
}

template<class TParam1>
zSignalEmitterAsync1<TParam1>::zSignalEmitterAsync1() 
: zSignalEmitterAsync(NULL) 
{
}

template<class TParam1>
zSignalEmitterAsync1<TParam1>::zSignalEmitterAsync1(zSignalEmitterAsyncGroup* _pEmitterGroup) 
: zSignalEmitterAsync(_pEmitterGroup)
{
}

template<class TParam1>
void zSignalEmitterAsync1<TParam1>::Emit(TParam1 _Param1)
{
	//! @todo Perf :	Pool allocator taking most of the processing time, optimize pool
	//!					Or could use internal array, which ends up much faster (but have to manage sorting freed elements)
	static zenMem::zAllocatorPool sMemPool("SignalEmitterAsync1 Signal", sizeof(SignalInfo), 256, 256);
	if( mpEmitterGroup )
	{
		SignalInfo* pPendingSignal	= zenNew(&sMemPool) SignalInfo;	
		//SignalInfo* pPendingSignal	= GetPool().Allocate();	
		pPendingSignal->mpEmitter	= this;
		pPendingSignal->mParam1		= _Param1;
		mpEmitterGroup->mlstPendingSignal.PushTail(*pPendingSignal);
	}
}

template<class TParam1>
void zSignalEmitterAsync1<TParam1>::SignalInfo::Send()
{
	zList<zSignal::ConnectionBase>::Iterator it = dynamic_cast<const zSignal*>(mpEmitter)->mlstListeners.GetHeadIt();
	while (*it)
	{
		zSignal1::Connection* pConnection = static_cast<zSignal1::Connection*>(*it);
		pConnection->Emit(mParam1);
		++it;
	}

	//GetPool().Deallocate( this );
}

//#################################################################################################
template<class TParam1, class TParam2>
zSignalEmitterAsync2<TParam1, TParam2>::zSignalEmitterAsync2() 
: zSignalEmitterAsync(NULL) 
{
}

template<class TParam1, class TParam2>
zSignalEmitterAsync2<TParam1, TParam2>::zSignalEmitterAsync2(zSignalEmitterAsyncGroup* _pEmitterGroup) 
: zSignalEmitterAsync(_pEmitterGroup)
{
}

template<class TParam1, class TParam2>
void zSignalEmitterAsync2<TParam1, TParam2>::Emit(TParam1 _Param1, TParam2 _Param2)
{
	static zenMem::zAllocatorPool sMemPool("SignalEmitterAsync2 Signal", sizeof(SignalInfo), 256, 256);	
	if( mpEmitterGroup )
	{
		SignalInfo* pPendingSignal	= zenNew(&sMemPool) SignalInfo;
		pPendingSignal->mpEmitter	= this;
		pPendingSignal->mParam1		= _Param1;
		pPendingSignal->mParam2		= _Param2;
		mpEmitterGroup->mlstPendingSignal.PushTail( *pPendingSignal );	
	}
}

template<class TParam1, class TParam2>
void zSignalEmitterAsync2<TParam1, TParam2>::SignalInfo::Send()
{
	zList<zSignal::ConnectionBase>::Iterator it = dynamic_cast<const zSignal*>(mpEmitter)->mlstListeners.GetHeadIt();
	while (*it)
	{
		zSignal2::Connection* pConnection = static_cast<zSignal2::Connection*>(*it);
		pConnection->Emit(mParam1, mParam2);
		++it;
	}
}

//#################################################################################################
template<class TParam1, class TParam2, class TParam3>
zSignalEmitterAsync3<TParam1, TParam2, TParam3>::zSignalEmitterAsync3() 
: zSignalEmitterAsync(NULL) 
{
}

template<class TParam1, class TParam2, class TParam3>
zSignalEmitterAsync3<TParam1, TParam2, TParam3>::zSignalEmitterAsync3(zSignalEmitterAsyncGroup* _pEmitterGroup) 
: zSignalEmitterAsync(_pEmitterGroup)
{
}

template<class TParam1, class TParam2, class TParam3>
void zSignalEmitterAsync3<TParam1, TParam2, TParam3>::Emit(TParam1 _Param1, TParam2 _Param2, TParam3 _Param3)
{
	static zenMem::zAllocatorPool sMemPool("SignalEmitterAsync2 Signal", sizeof(SignalInfo), 256, 256);	
	if( mpEmitterGroup )
	{
		SignalInfo* pPendingSignal	= zenNew(&sMemPool) SignalInfo;
		pPendingSignal->mpEmitter	= this;
		pPendingSignal->mParam1		= _Param1;
		pPendingSignal->mParam2		= _Param2;
		pPendingSignal->mParam3		= _Param3;
		mpEmitterGroup->mlstPendingSignal.PushTail( *pPendingSignal );	
	}
}

template<class TParam1, class TParam2, class TParam3>
void zSignalEmitterAsync3<TParam1, TParam2, TParam3>::SignalInfo::Send()
{
	zList<zSignal::ConnectionBase>::Iterator it = dynamic_cast<const zSignal*>(mpEmitter)->mlstListeners.GetHeadIt();
	while (*it)
	{
		zSignal3::Connection* pConnection = static_cast<zSignal3::Connection*>(*it);
		pConnection->Emit(mParam1, mParam2);
		++it;
	}
}

}} //namespace zen { namespace zenSig


#endif