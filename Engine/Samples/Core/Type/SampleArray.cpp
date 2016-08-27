#include "zenEngine.h"

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
		zArrayDynamic<zU32>	aDynamicA;
		zArrayDynamic<zU32>	aDynamicB;
		zArrayStatic<zU32>	aStaticA;
		zArrayStatic<zU32>	aStaticB;

		zU32 puValueDynamic[10] = {1000,1001,1002,1003,1004,1005,1006,1007,1008,1009};
		zU32 puValueStatic[10] = {2000,2001,2002,2003,2004,2005,2006,2007,2008,2009};

		aDynamicA.SetCount(40);
		aStaticA.SetCount(40);
		aDynamicB.Copy(puValueDynamic, zenArrayCount(puValueDynamic) );
		aStaticB.Copy(puValueStatic, zenArrayCount(puValueStatic) );
		
		for(zU32 i=0; i<40; ++i)
		{
			aDynamicA[i]	= 100+i;
			aStaticA[i]		= 200+i;
		}

		aDynamicA += aDynamicB;
		aDynamicA += aStaticA;
		aDynamicA += aStaticB;

		
		zenIO::Log(zenConst::keLog_Game, zenConst::kzLineA40);
		zenIO::Log(zenConst::keLog_Game, " Array Sorting");
		zenIO::Log(zenConst::keLog_Game, zenConst::kzLineA40);
		zArrayStatic<zU32> uTestSort;
		const zUInt pSourceData[] = {2,132,876,23,54,265,34,1,475,287,452,884,343,2,88,3,324,827,123,278};
		uTestSort.Copy( pSourceData, zenArrayCount(pSourceData) );
		zenIO::Log(zenConst::keLog_Game, "Unsorted  :");
		for(zUInt idx(0), count(uTestSort.Count()); idx<count; ++idx )
			zenIO::Printf(zenConst::keLog_Game, " %03i", uTestSort[idx] );

		uTestSort.Sort();
		zUInt uPrevValue(0);
		zenIO::Log(zenConst::keLog_Game, "Sorted    :");
		for(zUInt idx(0), count(uTestSort.Count()); idx<count; ++idx )
		{
			zenAssert(	(uPrevValue < uTestSort[idx]) ||					//Previous value always smaller than current
						(uPrevValue == uTestSort[idx] && uPrevValue==2));	//Or current is equal to previous when it's 2 (only double inserted in array)
			zenIO::Printf(zenConst::keLog_Game, " %03i", uTestSort[idx] );
			uPrevValue = uTestSort[idx];
		}
	}
}