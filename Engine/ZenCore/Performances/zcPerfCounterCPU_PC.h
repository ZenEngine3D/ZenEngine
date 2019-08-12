#pragma once

namespace zcPerf 
{

class EventCPU_PC : public EventCPU_Base
{
zenClassDeclare(EventCPU_PC, EventCPU_Base)
public:									
	virtual void	CPUStart();
	virtual void	CPUStop();

protected:
					EventCPU_PC(const zStringHash32& _EventName);
};

}

