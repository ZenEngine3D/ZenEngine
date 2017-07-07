#pragma once

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

zUInt zTask::GetElapsedUSec()
{
	return static_cast<zUInt>(muElapsed + (IsRunning() ? (zenSys::GetTimeUSec() - muProfilingTimer) : 0));
}

}}

