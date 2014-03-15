#pragma once
#ifndef __LibCor_System_Thread_h__
#define __LibCor_System_Thread_h__

//=================================================================================================
// Main Class / functions
//=================================================================================================
namespace CSys
{

template< class T >
class AWThread
{
	ZENClassDeclareNoParent(AWThread);

protected:
	//pointer to an object's method
	typedef zU32 (T::* Method)(void);

public:
	explicit AWThread(T* pObject, zU32 ( T::* method)(void))
	{
		mpObject = pObject;
		mMethod = method;
		mThreadID = 0;
	}

	~AWThread() { }

	virtual bool Start() { return false; }
	virtual bool Interrupt() { return false; }
	virtual void Sleep(zU32 _duration) { }
	virtual bool IsInterrupted()  { return false; }
	virtual bool IsRunning()  { return false; }

	virtual void Join() { }		//!< Blocks the calling thread until this thread has stopped.
	virtual void Detach() { }

	ZENInline zU32 GetThreadID()const { return mThreadID; }

protected:
	zU32 mThreadID;
	T* mpObject;
	Method mMethod;

	static zU32 Run(void* pThreadObj)
	{
		AWThread<T>* pThread = (AWThread<T>*)(pThreadObj);
		return (pThread->mpObject->*pThread->mMethod)();
	}

private:
	AWThread(const AWThread<T>& _other) { }
	AWThread<T>& operator = (const AWThread<T>& _other) { }
};

}

#include ZENHeaderPlatform( zbSysThread )

#endif