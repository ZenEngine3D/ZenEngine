#include "zbBase.h"

namespace zen { namespace zenSig
{

zSlot::~zSlot()
{	
	SlotDisconnectAll();		
}

void zSlot::SlotDisconnectAll()
{
	while( !mlstEmitters.IsEmpty() )
		zenDel(mlstEmitters.GetHead());
}

} } //namespace zen { namespace zenSig


