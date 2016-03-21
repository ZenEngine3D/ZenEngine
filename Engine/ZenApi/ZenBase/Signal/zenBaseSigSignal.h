#pragma once
#ifndef __zenApi_Base_Signal_Signal_h__
#define __zenApi_Base_Signal_Signal_h__

namespace zen { namespace zenSig
{

//=================================================================================================
//! @class		
//-------------------------------------------------------------------------------------------------
//! @brief		
//! @details	
//=================================================================================================	
class zSignal
{
ZENClassDeclareNoParent(zSignal)
public:
	class ConnectionBase
	{
	public:	
						ConnectionBase();
		virtual			~ConnectionBase(){};
		zSignal*		mpSignal;
		class zSlot*	mpObject;
	
	protected:
		zListLink		mlnkSignal;
		zListLink		mlnkSlot;
	public:
		typedef zList<ConnectionBase, &ConnectionBase::mlnkSignal, true>	ListSignal;
		typedef zList<ConnectionBase, &ConnectionBase::mlnkSlot, true>		ListSlot;
	};

	virtual							~zSignal();
	void							Disconnect(class zSlot& _Object);	
	bool							HasListeners()const;
	ConnectionBase::ListSignal		mlstListeners; //! @todo Urgent : Not have this public
protected:
	void							DisconnectAll();
	void							Connect( class zSlot& _Object, ConnectionBase& _Connection );

};

//=================================================================================================
//! @class		
//-------------------------------------------------------------------------------------------------
//! @brief		
//! @details	
//=================================================================================================	
class zSignal0 : public zSignal
{
ZENClassDeclare(zSignal0, zSignal)
protected:
	class Connection : public ConnectionBase {
	public:
		virtual void Emit() = 0;
	};

	template<class TSlotObject>
	class TConnection : public Connection
	{
	public:
		typedef void (TSlotObject::*Slot)();
		TConnection(TSlotObject& _SlotObject, Slot _Slot);
		virtual void Emit();

	protected:
		Slot mpObjectMethodSlot;
	};

	zSignal0(){};
public:
	template<class TSlotObject> 
	void Connect(TSlotObject& _Object, typename TConnection<TSlotObject>::Slot _Slot);
};

//=================================================================================================
//! @class		
//-------------------------------------------------------------------------------------------------
//! @brief		
//! @details	
//=================================================================================================	
template<class TParam1>
class zSignal1 : public zSignal
{
ZENClassDeclare(zSignal1, zSignal)
protected:
	class Connection : public ConnectionBase { 
	public:
		virtual void Emit(TParam1) = 0;
	};

	template<class TSlotObject>
	class TConnection : public Connection
	{	
	public:
	typedef void (TSlotObject::*Slot) (TParam1);
		TConnection(TSlotObject& _SlotObject, Slot _Slot);
		virtual void	Emit(TParam1);

	protected:
		Slot			mpObjectMethodSlot;
	};

	zSignal1(){};
public:	
	template<class TSlotObject> 
	void Connect( TSlotObject& _Object, typename TConnection<TSlotObject>::Slot _Slot );	
};

//=================================================================================================
//! @class		
//-------------------------------------------------------------------------------------------------
//! @brief		
//! @details	
//=================================================================================================	
template<class TParam1, class TParam2>
class zSignal2 : public zSignal
{
ZENClassDeclare(zSignal2, zSignal)
protected:
	class Connection : public ConnectionBase {
	public:
		virtual void Emit(TParam1, TParam2) = 0;
	};

	template<class TSlotObject>
	class TConnection : public Connection
	{
	public:
		typedef void (TSlotObject::*Slot) (TParam1, TParam2);
		TConnection(TSlotObject& _SlotObject, Slot _Slot);
		virtual void	Emit(TParam1, TParam2);

	protected:
		Slot			mpObjectMethodSlot;
	};

public:
	template<class TSlotObject> void	Connect(TSlotObject& _Object, typename TConnection<TSlotObject>::Slot _Slot);
};

//=================================================================================================
//! @class		
//-------------------------------------------------------------------------------------------------
//! @brief		
//! @details	
//=================================================================================================	
template<class TParam1, class TParam2, class TParam3>
class zSignal3 : public zSignal
{
ZENClassDeclare(zSignal3, zSignal)
protected:
	class Connection : public ConnectionBase {
	public:
		virtual void Emit(TParam1, TParam2, TParam3) = 0;
	};

	template<class TSlotObject>
	class TConnection : public Connection
	{
	public:
		typedef void (TSlotObject::*Slot) (TParam1, TParam2, TParam3);
		TConnection(TSlotObject& _SlotObject, Slot _Slot);
		virtual void	Emit(TParam1, TParam2, TParam3);

	protected:
		Slot			mpObjectMethodSlot;
	};

public:
	template<class TSlotObject> void	Connect(TSlotObject& _Object, typename TConnection<TSlotObject>::Slot _Slot);
};

} } //namespace zen { namespace zenSig

#endif
