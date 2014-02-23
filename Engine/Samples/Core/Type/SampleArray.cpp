#include "libZenEngine.h"

//=================================================================================================
//! @example SampleArray.cpp
//! Informations on the usage of various arrays functionalities
//=================================================================================================

namespace sample
{
	//==================================================================================================
	//! @brief		Test the result of static and dynamic arrays
	//==================================================================================================
	void SampleArray()
	{	
		zenArrayDynamic<zenU32>	aDynamicA;
		zenArrayDynamic<zenU32>	aDynamicB;
		zenArrayStatic<zenU32>	aStaticA;
		zenArrayStatic<zenU32>	aStaticB;

		zenU32 puValueDynamic[10] = {1000,1001,1002,1003,1004,1005,1006,1007,1008,1009};
		zenU32 puValueStatic[10] = {2000,2001,2002,2003,2004,2005,2006,2007,2008,2009};

		aDynamicA.SetCount(40);
		aStaticA.SetCount(40);
		aDynamicB.Copy(puValueDynamic, AWArrayCount(puValueDynamic) );
		aStaticB.Copy(puValueStatic, AWArrayCount(puValueStatic) );
		
		for(zenU32 i=0; i<40; ++i)
		{
			aDynamicA[i]	= 100+i;
			aStaticA[i]		= 200+i;
		}

		aDynamicA += aDynamicB;
		aDynamicA += aStaticA;
		aDynamicA += aStaticB;

		CMgr::Log.Log(CLog::keLog_Game, awconst::kzLineA40);
		CMgr::Log.Log(CLog::keLog_Game, " Array Sorting");
		CMgr::Log.Log(CLog::keLog_Game, awconst::kzLineA40);
		zenArrayStatic<zenU32> uTestSort;
		const zenUInt pSourceData[] = {2,132,876,23,54,265,34,1,475,287,452,884,343,2,88,3,324,827,123,278};
		uTestSort.Copy( pSourceData, AWArrayCount(pSourceData) );
		CMgr::Log.Log(CLog::keLog_Game, "Unsorted  :");
		for(zenUInt idx(0), count(uTestSort.Count()); idx<count; ++idx )
			CMgr::Log.Printf(CLog::keLog_Game, " %03i", uTestSort[idx] );
		
		uTestSort.Sort();
		zenUInt uPrevValue(0);
		CMgr::Log.Log(CLog::keLog_Game, "Sorted    :");
		for(zenUInt idx(0), count(uTestSort.Count()); idx<count; ++idx )
		{
			AWAssert(	(uPrevValue < uTestSort[idx]) ||					//Previous value always smaller than current
						(uPrevValue == uTestSort[idx] && uPrevValue==2));	//Or current is equal to previous when it's 2 (only double inserted in array)
			CMgr::Log.Printf(CLog::keLog_Game, " %03i", uTestSort[idx] );
		}
	}
}