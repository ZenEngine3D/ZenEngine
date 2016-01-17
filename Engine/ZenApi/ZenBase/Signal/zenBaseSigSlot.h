#pragma once
#ifndef __zenApi_Base_Signal_Slot_h__
#define __zenApi_Base_Signal_Slot_h__

namespace zen {	namespace zenSig	
{

//=================================================================================================
//! @class		zSlot
//-------------------------------------------------------------------------------------------------
//! @brief		Class wanting to received signal, must derive from this
//! @details	
//=================================================================================================
class zSlot
{
public:
	virtual ~zSlot();
	void	SlotDisconnectAll();

protected:
	zSignal::ConnectionBase::ListSlot mlstEmitters;
	friend class zSignal;
};

	
}	} //namespace zen { namespace zenSig

#endif
