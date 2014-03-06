#include "libZenEngine.h"

//=================================================================================================
//! @example SampleHash.cpp
//! Informations on the usage of various string Hashing functionalities
//=================================================================================================

namespace sample
{
	class TestNoMemCopy
	{
		zenU32 mTestVal;
	};

	class TestWithMemCopy
	{
		zenU32 mTestVal;
	};
	ZENSupportMemcopy(TestWithMemCopy);

	//==================================================================================================
	//! @brief		Test misc type functionalities
	//==================================================================================================
	void SampleTypeMisc()
	{	
		CMgr::Log.Log(CLog::keLog_Game, zenConst::kzLineA40);
		CMgr::Log.Log(CLog::keLog_Game, " TYPE MISC");
		CMgr::Log.Log(CLog::keLog_Game, zenConst::kzLineA40);

		//-------------------------------------------------
		// Verify compile time type/class informations
		bool bPointerFalse	= ZenIsPointer<int>::value;	
		bool bPointerTrue	= ZenIsPointer<int*>::value;
		bool bMemcpyTrue1	= ZenSupportsMemCopy<zenU8>::value;
		bool bMemcpyTrue2	= ZenSupportsMemCopy<TestWithMemCopy>::value;
		bool bMemcpyFalse	= ZenSupportsMemCopy<TestNoMemCopy>::value;

		
		CMgr::Log.Log(CLog::keLog_Game, "Compile Type Info :");
		CMgr::Log.Log(CLog::keLog_Game, "   ZenIsPointer<int>                    = 0 ? %s", zenConst::kzFalseTrue[bPointerFalse==false] );
		CMgr::Log.Log(CLog::keLog_Game, "   ZenIsPointer<int*>                   = 1 ? %s", zenConst::kzFalseTrue[bPointerTrue==true] );
		CMgr::Log.Log(CLog::keLog_Game, "   ZenSupportsMemCopy<TestNoMemCopy>    = 0 ? %s", zenConst::kzFalseTrue[bMemcpyFalse==false] );
		CMgr::Log.Log(CLog::keLog_Game, "   ZenSupportsMemCopy<zenU8>             = 1 ? %s", zenConst::kzFalseTrue[bMemcpyTrue1==true] );
		CMgr::Log.Log(CLog::keLog_Game, "   ZenSupportsMemCopy<TestWithMemCopy>  = 1 ? %s", zenConst::kzFalseTrue[bMemcpyTrue2==true] );
	}

}