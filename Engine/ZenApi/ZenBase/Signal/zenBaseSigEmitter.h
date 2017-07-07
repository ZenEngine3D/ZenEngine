#pragma once

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
zenClassDeclare(zSignalEmitter0, zSignal)
public:
	zenInline void Emit()const;
	zenInline void DisconnectAll();
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
zenClassDeclare(zSignalEmitter1, zSignal1)
public:
	zenInline void Emit(TParam1)const;
	zenInline void DisconnectAll();
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
zenClassDeclare(zSignalEmitter2, zSignal2)
public:
	zenInline void Emit(TParam1, TParam2)const;
	zenInline void DisconnectAll();
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
zenClassDeclare(zSignalEmitter3, zSignal3)
public:
	zenInline void Emit(TParam1, TParam2, TParam3)const;
	zenInline void DisconnectAll();
};


} } //namespace zen { namespace zenSig

