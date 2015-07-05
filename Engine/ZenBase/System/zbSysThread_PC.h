#pragma once
#ifndef __zBase_System_Thread_PC_h__
#define __zBase_System_Thread_PC_h__

namespace zbSys
{
/*
template< class T >
class AWThread_PC : public AWThread<T>
{
	ZENClassDeclare(AWThread_PC, AWThread);

public:
	explicit AWThread_PC(T* pObject, zU32 ( T::* method)(void)) : 
	AWThread<T>(pObject, method)
	{
		mhThread = NULL;
		mhInterrupt = CreateSemaphore(NULL, 1, 1, NULL);
		mhSingleStart = CreateMutex(NULL, FALSE, NULL);
	}
	virtual ~AWThread_PC()
	{
		if(mhInterrupt)
			CloseHandle(mhInterrupt);
		
		if(mhThread)
			CloseHandle(mhThread);
	}

	virtual bool Start()
	{
		__try 
		{
			if(WaitForSingleObject(mhSingleStart, 0) != WAIT_OBJECT_0)
			{
				return false;
			}

			if(mhThread)    // Thread had been started sometime in the past
			{
				if (WaitForSingleObject(mhThread, 0) == WAIT_TIMEOUT)
				{   // if thread's still running deny new start
					return false;
				}
				CloseHandle(mhThread);
			}
			// (Re-)Set not interrupted semaphore state
			WaitForSingleObject(mhInterrupt, 0);

			mhThread = CreateThread( NULL, 0, (LPTHREAD_START_ROUTINE) AWThread_PC<T>::Run, this, 0, (LPDWORD)(&mThreadID) );

			if(mhThread)
			{
				return true;
			}

			return false;
		}
		__finally
		{
			ReleaseMutex(mhSingleStart);
		}
	}
	virtual bool Interrupt()
	{
		if(mhInterrupt)
		{
			return ((ReleaseSemaphore(mhInterrupt, 1, NULL) == FALSE) ? false : true);
		}

		return false;
	}

	virtual void Sleep(zU32 _duration)
	{
		//TODO
	}

	virtual bool IsInterrupted()
	{
		if(WaitForSingleObject(mhInterrupt, 0) == WAIT_TIMEOUT)
			return false;
		ReleaseSemaphore(mhInterrupt, 1, NULL);  // keep interrupted state
		return true;
	}
	virtual bool IsRunning()
	{
		DWORD exitCode = 0;
		if(mhThread)
			GetExitCodeThread(mhThread, &exitCode);
		if(exitCode == STILL_ACTIVE)
			return true;
		return false;
	}

	virtual void Join()		//!< Blocks the calling thread until this thread has stopped.
	{
		WaitForSingleObject(mhThread, INFINITE);
	}

	virtual void Detach()
	{
		//TODO
	}

private:
	HANDLE mhThread;
	HANDLE mhInterrupt;
	HANDLE mhSingleStart;

private:
	AWThread_PC(const AWThread_PC<T>& _other) { }
	AWThread_PC<T>& operator = (const AWThread_PC<T>& _other) { }
};
*/
}

#endif