#pragma once
#ifndef __LibZenBase_Task_Task_h__
#define __LibZenBase_Task_Task_h__

namespace CJob
{

class JobItem : public zenList2xNode
{
ZENClassDeclare(JobItem, zenList2xNode)
public:
	enum eStatus{ keStatus_Wait, keStatus_Ready, keStatus_Running, keStatus_Success, keStatus_FailStart, keStatus_FailExe, keStatus_FailEnd, keStatus__Count };
	
	JobItem()
	: meStatus(keStatus_Wait)
	, mbAutoCleanup(true)
	, mTimeAdded(0)
	, mTimeStart(0)
	, mTimeStop(0)
	{	
	}
	virtual			~JobItem(){}

	virtual bool	Start(){ meStatus = keStatus_Ready; return true; }
	virtual bool	THRTask_Execute()=0;		
	virtual bool	End(){return true;}
	virtual void	Cleanup(){ delete this; }
	bool			IsFailed(){return meStatus >= keStatus_FailStart;}	
	eStatus			meStatus;		//!< Current task status
	bool			mbAutoCleanup;	//!< If task should be cleanup after completion
	zenTimeStamp		mTimeAdded;		//!< Time task was added to the queue
	zenTimeStamp		mTimeStart;		//!< Time task was started
	zenTimeStamp		mTimeStop;		//!< Time task was completed
};

}  

#endif