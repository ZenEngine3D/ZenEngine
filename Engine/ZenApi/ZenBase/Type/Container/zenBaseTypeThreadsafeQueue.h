#pragma once
#ifndef __zenApi_Base_Type_ThreadsafeQueue_h__
#define __zenApi_Base_Type_ThreadsafeQueue_h__

#include <atomic>

namespace zen { namespace zenType 
{
	//! @todo performance Create better lockless queue that doesn't involve 'locking' with atomic :P
	template<class TObject, zUInt TItemCount>
	class zThreadsafeQueueFifo
	{
	public:
							zThreadsafeQueueFifo();
		void				Push(const TObject& _Item);
		bool				Pop(TObject& _Item);

	protected:
		std::atomic<zU32>	mLock;
		zU64				mNextFree;
		zU64				mCurrent;
		TObject				mQueue[TItemCount];
	};	

} } //namespace zen, Type

#include "zenBaseTypeThreadsafeQueue.inl"

#endif