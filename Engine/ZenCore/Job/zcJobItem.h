#pragma once
#ifndef __zCore_Job_Item_h__
#define __zCore_Job_Item_h__

namespace zcJob
{

class JobItem : public zList2xNode
{
ZENClassDeclare(JobItem, zList2xNode)
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
	zTimeStamp		mTimeAdded;		//!< Time task was added to the queue
	zTimeStamp		mTimeStart;		//!< Time task was started
	zTimeStamp		mTimeStop;		//!< Time task was completed
};

}  

#endif
