#pragma once
#ifndef __zBase_System_Fiber_h__
#define __zBase_System_Fiber_h__

#include "zbSysFiber_base.h"
#include ZENHeaderPlatform( zbSysFiber )


//=================================================================================================
// Main Class / functions
//=================================================================================================
namespace zbSys
{

class zbFiber : public zbFiber_Platform
{
zenClassDeclare(zbFiber, zbFiber_Platform);
public:
	enum eConst{ keFiberCount = 256, keThreadMax=32 };

public:
								zbFiber(zUInt _StackSize);
	void						Activate(zUInt _uThreadId, zenSys::zTask* _pTask=nullptr);
	void						Deactivate();	
	void						Suspend();
	void						InfiniteLoop();
	
protected:	
	ZENInline bool				CanResume(){ return mpRunningTask && mpRunningTask->CanResume(); }	
	zenSys::zTask*				mpRunningTask;	
	
protected:
	static void					AddFreeFiber(zbFiber* _pFreeFiber);
	static void					AddPendingTask(zenSys::zTask* _pTask);
	friend void					FiberUpdateLoop(zUInt _uThreadId);
	friend class				zenSys::zTask;
};

void StartTaskProcessing();
void FiberUpdateLoop(zUInt _uThreadId);

}



#endif