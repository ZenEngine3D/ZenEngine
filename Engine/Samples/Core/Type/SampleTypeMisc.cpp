#include "libZenEngine.h"

//=================================================================================================
//! @example SampleHash.cpp
//! Informations on the usage of various string Hashing functionalities
//=================================================================================================

namespace sample
{
	class TestNoMemCopy
	{
		zU32 mTestVal;
	};

	class TestWithMemCopy
	{
		zU32 mTestVal;
	};
	ZENSupportMemcopy(TestWithMemCopy);

	//==================================================================================================
	//! @brief		Test misc type functionalities
	//==================================================================================================
	void SampleTypeMisc()
	{	
		zbMgr::Log.Log(zbLog::keLog_Game, zenConst::kzLineA40);
		zbMgr::Log.Log(zbLog::keLog_Game, " TYPE MISC");
		zbMgr::Log.Log(zbLog::keLog_Game, zenConst::kzLineA40);

		//-------------------------------------------------
		// Verify compile time type/class informations
		bool bPointerFalse	= ZenIsPointer<int>::value;	
		bool bPointerTrue	= ZenIsPointer<int*>::value;
		bool bMemcpyTrue1	= ZenSupportsMemCopy<zU8>::value;
		bool bMemcpyTrue2	= ZenSupportsMemCopy<TestWithMemCopy>::value;
		bool bMemcpyFalse	= ZenSupportsMemCopy<TestNoMemCopy>::value;

		
		zbMgr::Log.Log(zbLog::keLog_Game, "Compile Type Info :");
		zbMgr::Log.Log(zbLog::keLog_Game, "   ZenIsPointer<int>                    = 0 ? %s", zenConst::kzFalseTrue[bPointerFalse==false] );
		zbMgr::Log.Log(zbLog::keLog_Game, "   ZenIsPointer<int*>                   = 1 ? %s", zenConst::kzFalseTrue[bPointerTrue==true] );
		zbMgr::Log.Log(zbLog::keLog_Game, "   ZenSupportsMemCopy<TestNoMemCopy>    = 0 ? %s", zenConst::kzFalseTrue[bMemcpyFalse==false] );
		zbMgr::Log.Log(zbLog::keLog_Game, "   ZenSupportsMemCopy<zU8>             = 1 ? %s", zenConst::kzFalseTrue[bMemcpyTrue1==true] );
		zbMgr::Log.Log(zbLog::keLog_Game, "   ZenSupportsMemCopy<TestWithMemCopy>  = 1 ? %s", zenConst::kzFalseTrue[bMemcpyTrue2==true] );
	}

}