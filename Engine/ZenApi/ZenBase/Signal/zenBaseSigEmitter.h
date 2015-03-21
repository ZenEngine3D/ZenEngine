#pragma once
#ifndef __zenApi_Base_Signal_Emitter_h__
#define __zenApi_Base_Signal_Emitter_h__

namespace zen { namespace zenSig
{

//=================================================================================================
//! @class		
//-------------------------------------------------------------------------------------------------
//! @brief		
//! @details	
//=================================================================================================	
class zSignalEmitter0 : public zSignal0
{
ZENClassDeclare(zSignalEmitter0, zSignal)
public:
	ZENInline void Emit()const;
	ZENInline void DisconnectAll();
};

//=================================================================================================
//! @class		
//-------------------------------------------------------------------------------------------------
//! @brief		
//! @details	
//=================================================================================================	
template<class TParam1>
class zSignalEmitter1 : public zSignal1<TParam1>
{
ZENClassDeclare(zSignalEmitter1, zSignal1)
public:
	ZENInline void Emit(TParam1)const;
	ZENInline void DisconnectAll();
};

//=================================================================================================
//! @class		
//-------------------------------------------------------------------------------------------------
//! @brief		
//! @details	
//=================================================================================================	
template<class TParam1, class TParam2>
class zSignalEmitter2 : public zSignal2<TParam1, TParam2>
{
ZENClassDeclare(zSignalEmitter2, zSignal2)
public:
	ZENInline void Emit(TParam1, TParam2)const;
	ZENInline void DisconnectAll();
};

//=================================================================================================
//! @class		
//-------------------------------------------------------------------------------------------------
//! @brief		
//! @details	
//=================================================================================================	
template<class TParam1, class TParam2, class TParam3>
class zSignalEmitter3 : public zSignal3<TParam1, TParam2, TParam3>
{
ZENClassDeclare(zSignalEmitter3, zSignal3)
public:
	ZENInline void Emit(TParam1, TParam2, TParam3)const;
	ZENInline void DisconnectAll();
};


} } //namespace zen { namespace zenSig

#endif
