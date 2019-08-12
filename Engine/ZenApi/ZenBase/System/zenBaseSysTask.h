#pragma once

namespace zbSys { class zbFiber; } 

namespace zen { namespace zenSys 
{

class zTask
{
zenClassDeclareNoParent(zTask);
public:	
								zTask();
	virtual void				Run()=0;
	virtual void				Cleanup();
	
	void						AddToPending(std::atomic<zI32>* _pGateParent=nullptr);

	void						Suspend();
	
	zenInline bool				IsRunning();
	zenInline bool				CanResume();
	zenInline zUInt				GetElapsedUSec();
	static zUInt				GetCPUCoresize();

	void						ProcessingStart();
	void						ProcessingStop();

	class zbSys::zbFiber*		mpOwnerFiber;
	zUInt						muElapsed;
	zU64						muProfilingTimer;
	std::atomic<bool>			mbRunning;
	std::atomic<zI32>			mGate;
	std::atomic<zI32>*			mpGateParent;
};

void StartTaskProcessing(); //! @todo clean do not export this, make part of engine init

}}

#include "zenBaseSysTask.inl"

