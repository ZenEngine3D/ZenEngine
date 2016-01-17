#include "zbBase.h"

namespace zen { namespace zenSig
{

zSignal::ConnectionBase::ConnectionBase()
: mpSignal(nullptr)
{
}

zSignal::~zSignal()
{
	DisconnectAll();
}

void zSignal::Connect(zSlot& _Object, zSignal::ConnectionBase& _Connection)
{
	_Connection.mpSignal = this;
	_Connection.mpObject = &_Object;
	mlstListeners.PushHead(_Connection);
	_Object.mlstEmitters.PushHead(_Connection);
}

void zSignal::Disconnect(zSlot& _Object)
{
	auto it = _Object.mlstEmitters.GetHeadIt();
	while( *it )
	{
		if( it->mpSignal == this )
		{
			zSignal::ConnectionBase::ListSignal::Remove(**it);
			zSignal::ConnectionBase::ListSlot::Remove(**it);
			break;
		}		
	}
}

void zSignal::DisconnectAll()
{
	while (!mlstListeners.IsEmpty())
		zenDel(mlstListeners.GetHead());
}

bool zSignal::HasListeners()const
{
	return mlstListeners.IsEmpty() == false;
}


} } //namespace zen { namespace zenSig