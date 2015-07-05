#pragma once
#ifndef __zenApi_Base_System_Task_inl__
#define __zenApi_Base_System_Task_inl__

namespace zen { namespace zenSys 
{

bool zTask::CanResume()
{
	return mGate.load() <= 0;
}

bool zTask::IsRunning()
{
	return mbRunning.load() == true;
}

zU32 zTask::GetElapsedUSec()
{
	return muElapsed + (IsRunning() ? zU32(zenSys::GetTimeUSec() - muProfilingTimer) : 0);
}

}}



#endif
