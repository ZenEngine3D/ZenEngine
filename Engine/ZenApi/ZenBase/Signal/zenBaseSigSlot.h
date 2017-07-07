#pragma once

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

