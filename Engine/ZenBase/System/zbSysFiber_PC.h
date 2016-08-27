#pragma once
#ifndef __zbSystem_Fiber_PC_h__
#define __zbSystem_Fiber_PC_h__

namespace zbSys
{

class zbFiber_PC : public zbFiber_Base
{
zenClassDeclare(zbFiber_PC, zbFiber_Base);
public:
					zbFiber_PC(zUInt _StackSize);
					~zbFiber_PC();

	void			HALActivate();
	void			HALDeactivate();

protected:
	LPVOID			mpFiber;	
};

typedef zbFiber_PC zbFiber_Platform;

}

#endif