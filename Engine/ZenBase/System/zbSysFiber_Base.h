#pragma once

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
