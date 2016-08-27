#pragma once
#ifndef __zBase_System_Fiber_Base_h__
#define __zBase_System_Fiber_Base_h__

//=================================================================================================
// Main Class / functions
//=================================================================================================
namespace zbSys
{

class zbFiber_Base
{
zenClassDeclareNoParent(zbFiber_Base);
public:
						zbFiber_Base(zUInt _uStackSize);
protected:
	zenSys::zTask*		mpRunningTask;
	zUInt				muThreadId;
	zUInt				muStackSize;
};

}

#endif