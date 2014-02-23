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
	AWAddSupportsMemCopy(TestWithMemCopy);

	//==================================================================================================
	//! @brief		Test misc type functionalities
	//==================================================================================================
	void SampleTypeMisc()
	{	
		CMgr::Log.Log(CLog::keLog_Game, awconst::kzLineA40);
		CMgr::Log.Log(CLog::keLog_Game, " TYPE MISC");
		CMgr::Log.Log(CLog::keLog_Game, awconst::kzLineA40);

		//-------------------------------------------------
		// Verify compile time type/class informations
		bool bPointerFalse	= AWIsPointer<int>::value;	
		bool bPointerTrue	= AWIsPointer<int*>::value;
		bool bMemcpyTrue1	= AWSupportsMemCopy<zenU8>::value;
		bool bMemcpyTrue2	= AWSupportsMemCopy<TestWithMemCopy>::value;
		bool bMemcpyFalse	= AWSupportsMemCopy<TestNoMemCopy>::value;

		
		CMgr::Log.Log(CLog::keLog_Game, "Compile Type Info :");
		CMgr::Log.Log(CLog::keLog_Game, "   AWIsPointer<int>                    = 0 ? %s", awconst::kzFalseTrue[bPointerFalse==false] );
		CMgr::Log.Log(CLog::keLog_Game, "   AWIsPointer<int*>                   = 1 ? %s", awconst::kzFalseTrue[bPointerTrue==true] );
		CMgr::Log.Log(CLog::keLog_Game, "   AWSupportsMemCopy<TestNoMemCopy>    = 0 ? %s", awconst::kzFalseTrue[bMemcpyFalse==false] );
		CMgr::Log.Log(CLog::keLog_Game, "   AWSupportsMemCopy<zenU8>             = 1 ? %s", awconst::kzFalseTrue[bMemcpyTrue1==true] );
		CMgr::Log.Log(CLog::keLog_Game, "   AWSupportsMemCopy<TestWithMemCopy>  = 1 ? %s", awconst::kzFalseTrue[bMemcpyTrue2==true] );
	}

}