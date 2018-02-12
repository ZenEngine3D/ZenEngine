namespace zen { namespace zenSig
{

template<typename... TParameters>
zSignal<TParameters...>::Slot::Slot()
{
	mpNext = mpPrev = nullptr;	
}

template<typename... TParameters>
zSignal<TParameters...>::Slot::~Slot()
{
	Disconnect();
}

template<typename... TParameters>
void zSignal<TParameters...>::Slot::Disconnect()
{
	if( mpNext != nullptr )
	{
		decltype(zSignal::mlstSlots)::remove(*this);
		mpNext = nullptr;
		mpPrev = nullptr;
	}	
}

template<typename... TParameters>
void zSignal<TParameters...>::Slot::Connect(zSignal& inSignal, const typename Callback& inCallback)
{
	mCallback = inCallback;
	inSignal.mlstSlots.push_back(*this);
}

template<typename... TParameters>
const typename zSignal<TParameters...>::Slot::Callback& zSignal<TParameters...>::Slot::GetCallback()const
{
	return mCallback;
}

template<typename... TParameters>
void zSignal<TParameters...>::Emit(TParameters...inValues)const
{
	auto it = mlstSlots.begin();
	while( it != mlstSlots.end() )
	{
		const Slot& slot = *it++; //Increment before invoking callback, so if callback removes slot, won't affect iteration
		slot.GetCallback()(inValues...);
	}	
}

template<typename... TParameters>
void zSignal<TParameters...>::DisconnectAll()
{
	mlstSlots.Empty();
}

}} //namespace zen { namespace zenSig
