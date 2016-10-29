#pragma once
#ifndef __zenApi_Base_Signal_EmitterAsync_h__
#define __zenApi_Base_Signal_EmitterAsync_h__

//! @todo clean : See if c++11 could make varg simpler instead of 1 class per arg count

namespace zen { namespace zenSig
{

//=================================================================================================
//! @class		
//-------------------------------------------------------------------------------------------------
//! @brief		
//! @details	
//=================================================================================================	
class zSignalEmitterAsync
{
zenClassDeclareNoParent(zSignalEmitterAsync)
public:
						zSignalEmitterAsync();
						zSignalEmitterAsync(class zSignalEmitterAsyncGroup* _pEmitterGroup);
	virtual				~zSignalEmitterAsync();
	void				ClearPendingSignals();													//!< @Brief Delete all pending signals owned by this emitter
	void				SetEmitterGroup(class zSignalEmitterAsyncGroup* _pEmitterGroup);		//!< @Brief Assign group this emitters belongs to

protected:
	struct SignalInfo
	{
		zSignalEmitterAsync*	mpEmitter;		
		zListLink				mlnkPendingInGroup;
		zListLink				mlnkPendingInEmitter;
		virtual void			Send() = 0;

		typedef zList<SignalInfo, &SignalInfo::mlnkPendingInGroup, false>	ListGroup;
		typedef zList<SignalInfo, &SignalInfo::mlnkPendingInEmitter, false>	ListEmitter;
	};
	class zSignalEmitterAsyncGroup*	mpEmitterGroup;
	SignalInfo::ListEmitter			mlstPendingSignal;
	friend class zSignalEmitterAsyncGroup;
};

class zSignalEmitterAsyncGroup
{
public:
					zSignalEmitterAsyncGroup();
	virtual			~zSignalEmitterAsyncGroup();
	void			EmitAllSignals();				//!< Send all signal that have been emitted in this group
	void			ClearPendingSignals();			//!< @Brief Delete all pending signal owned by this emitter group
//protected:
	zSignalEmitterAsync::SignalInfo::ListGroup mlstPendingSignal;
};

//=================================================================================================
//! @class		
//-------------------------------------------------------------------------------------------------
//! @brief		
//! @details	
//=================================================================================================	
class zSignalEmitterAsync0 : public zSignal0, public zSignalEmitterAsync
{
zenClassDeclare(zSignalEmitterAsync0, zSignal0)
public:
	zSignalEmitterAsync0()
	: zSignalEmitterAsync(nullptr)
	{
	}

	zSignalEmitterAsync0(zSignalEmitterAsyncGroup* _pEmitterGroup)
	: zSignalEmitterAsync(_pEmitterGroup)
	{
	}

	zenInline void Emit();

	struct SignalInfo : public zSignalEmitterAsync::SignalInfo
	{
		virtual void Send();
	};
};

//=================================================================================================
//! @class		
//-------------------------------------------------------------------------------------------------
//! @brief		
//! @details	
//=================================================================================================	
template<class TParam1>
class zSignalEmitterAsync1 : public zSignal1<TParam1>, public zSignalEmitterAsync
{
zenClassDeclare(zSignalEmitterAsync1, zSignal1)
public:
					zSignalEmitterAsync1();
					zSignalEmitterAsync1(zSignalEmitterAsyncGroup* _pEmitterGroup);
	zenInline void	Emit(TParam1);	

	struct SignalInfo : public zSignalEmitterAsync::SignalInfo
	{
		TParam1 mParam1;
		virtual void Send();
	};

	static zenMem::zAllocatorFastPool< typename zSignalEmitterAsync1<TParam1>::SignalInfo, true, false >& GetPool(); //! testing fast pool
};

//=================================================================================================
//! @class		
//-------------------------------------------------------------------------------------------------
//! @brief		
//! @details	
//=================================================================================================	
template<class TParam1, class TParam2>
class zSignalEmitterAsync2 : public zSignal2<TParam1, TParam2>, public zSignalEmitterAsync
{
zenClassDeclare(zSignalEmitterAsync2, zSignal2)
public:	
					zSignalEmitterAsync2();
					zSignalEmitterAsync2(zSignalEmitterAsyncGroup* _pEmitterGroup);
	zenInline void	Emit(TParam1, TParam2);

	struct SignalInfo : public zSignalEmitterAsync::SignalInfo
	{
		TParam1 mParam1;
		TParam1 mParam2;
		virtual void Send();
	};
};

//=================================================================================================
//! @class		
//-------------------------------------------------------------------------------------------------
//! @brief		
//! @details	
//=================================================================================================	
template<class TParam1, class TParam2, class TParam3>
class zSignalEmitterAsync3 : public zSignal3<TParam1, TParam2, TParam3>, public zSignalEmitterAsync
{
zenClassDeclare(zSignalEmitterAsync3, zSignalEmitter3)
public:	
					zSignalEmitterAsync3();
					zSignalEmitterAsync3(zSignalEmitterAsyncGroup* _pEmitterGroup);
	zenInline void	Emit(TParam1, TParam2, TParam3);

	struct SignalInfo : public zSignalEmitterAsync::SignalInfo
	{
		TParam1 mParam1;
		TParam1 mParam2;
		TParam1 mParam3;
		virtual void Send();
	};
};

} } //namespace zen { namespace zenSig

#endif
