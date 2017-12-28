
namespace zen { namespace zenType {

	//================================================================================================
	//
	//================================================================================================
	template<class TObject, zUInt TItemCount>
	zThreadsafeQueueFifo<TObject, TItemCount>::zThreadsafeQueueFifo()
	: mNextFree( 0 )
	, mCurrent(0)
	, mLock(0)
	{
	}

	template<class TObject, zUInt TItemCount>
	void zThreadsafeQueueFifo<TObject, TItemCount>::Push(const TObject& _Item)
	{	
		while( mLock.exchange(1) != 0 );
		
		zenAssertMsg(mNextFree-mCurrent <= TItemCount, "Queue is at full capacity, increase its size");
		mQueue[mNextFree%TItemCount]	= _Item;
		++mNextFree;
		mLock.store(0);
	}

	template<class TObject, zUInt TItemCount>
	bool zThreadsafeQueueFifo<TObject, TItemCount>::Pop(TObject& _Item)
	{		
		while( mLock.exchange(1) > 0 );
		
		if(mCurrent == mNextFree)
		{
			mLock.store(0);
			return false;
		}
		
		_Item = mQueue[mCurrent%TItemCount];
		++mCurrent;
		mLock.store(0);
		return true;
	}

} } //namespace zen, Type

