#include "zenEngine.h"

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
		zenIO::Log(zenConst::keLog_Game, zenConst::kzLineA40);
		zenIO::Log(zenConst::keLog_Game, " TYPE MISC");
		zenIO::Log(zenConst::keLog_Game, zenConst::kzLineA40);

		//-------------------------------------------------
		// Verify compile time type/class informations
		bool bPointerFalse	= zenIsPointer<int>::value;	
		bool bPointerTrue	= zenIsPointer<int*>::value;
		bool bMemcpyTrue1	= zenSupportsMemCopy<zU8>::value;
		bool bMemcpyTrue2	= zenSupportsMemCopy<TestWithMemCopy>::value;
		bool bMemcpyFalse	= zenSupportsMemCopy<TestNoMemCopy>::value;
		
		zenIO::Log(zenConst::keLog_Game, "Compile Type Info :");
		zenIO::Log(zenConst::keLog_Game, "   zenIsPointer<int>                    = 0 ? %s", zenConst::kzFalseTrue[bPointerFalse==false] );
		zenIO::Log(zenConst::keLog_Game, "   zenIsPointer<int*>                   = 1 ? %s", zenConst::kzFalseTrue[bPointerTrue==true] );
		zenIO::Log(zenConst::keLog_Game, "   zenSupportsMemCopy<TestNoMemCopy>    = 0 ? %s", zenConst::kzFalseTrue[bMemcpyFalse==false] );
		zenIO::Log(zenConst::keLog_Game, "   zenSupportsMemCopy<zU8>              = 1 ? %s", zenConst::kzFalseTrue[bMemcpyTrue1==true] );
		zenIO::Log(zenConst::keLog_Game, "   zenSupportsMemCopy<TestWithMemCopy>  = 1 ? %s", zenConst::kzFalseTrue[bMemcpyTrue2==true] );
	}

}