#pragma once

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

}
