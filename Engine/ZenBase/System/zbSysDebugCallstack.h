#pragma once

//=================================================================================================
// Main Class / functions
//=================================================================================================
namespace zbSys
{

class Callstack
{
public:
	struct Entry
	{
		const char*	mFunctionName	= nullptr;
		const char* mFileName		= nullptr;
		zU32		mFileLine		= 0;
	};

	const Entry& GetEntry(zU32 _Depth) const
	{
		zenAssert(_Depth < mStackDepth)
		return maStackEntries[_Depth];
	}

	static const Callstack* GetAddCallstack(zUInt _IgnoreTopCount);

protected:
	Callstack(zU32 _StackDepth) : mStackDepth(_StackDepth){};			
	zU32	mStackDepth = 0;
	Entry	maStackEntries[zenVariableSize];
};

}

#include zenHeaderPlatform(zbSysDebugCallstack)
