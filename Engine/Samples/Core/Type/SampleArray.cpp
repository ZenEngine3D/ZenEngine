#include "zenEngine.h"

//=================================================================================================
//! @example SampleArray.cpp
//! Informations on the usage of various arrays functionalities
//=================================================================================================

namespace sample
{
	class ClassTestResize
	{
	public:
		__declspec(noinline)  ClassTestResize(int value, int value2)
		{
			mVal = 	value + value2;
			sAlive++;	
		}
		__declspec(noinline)  ClassTestResize()
		{
			mVal = 	sAlive++;	
		}
		__declspec(noinline)  ~ClassTestResize()
		{
			--sAlive;
		}
		zI32 mVal;
		static zI32 sAlive;
	};
	zI32 ClassTestResize::sAlive = 0;

	//==================================================================================================
	//! @brief		Test the result of static and dynamic arrays
	//==================================================================================================
	void SampleArray()
	{		
		zArrayFixed<zU32,1> ArrayFixed1;
		zArrayFixed<zU32,2> ArrayFixed2;
		zArrayFixed<zU32,3> ArrayFixed3;
		zArrayFixed<zU32,4> ArrayFixed4;
		zArrayFixed<zU32,5> ArrayFixed5;
		zArrayFixed<zU32,16> ArrayFixed;
		zU32 sum(0);
		zU32 Counter(0);
		auto a = ArrayFixed.begin();
		auto b = ArrayFixed.end();
		for(auto& Val : ArrayFixed)
			Val = Counter++;

		zArrayDyn<zU32> ArrayDyn;
		ArrayDyn.resize(16);
		for (auto& Val : ArrayDyn)
			Val = Counter++;
		ArrayDyn.resize(20, 12);
		ArrayDyn.resize(32);
		ArrayDyn.resize(30);
		ArrayDyn.resize(10);

		zArrayDyn<ClassTestResize> ArrayObjDyn;
		ArrayObjDyn.resize(16);
		ArrayObjDyn.resize(20);
		ArrayObjDyn.resize(32);
		ArrayObjDyn.resize(30);
		ArrayObjDyn.resize(10);

#if 0
		zArrayDyn<zU32>	aDynamicA;
		zArrayDyn<zU32>	aDynamicB;
		zArrayStatic<zU32>	aStaticA;
		zArrayStatic<zU32>	aStaticB;

		zU32 puValueDynamic[10] = {1000,1001,1002,1003,1004,1005,1006,1007,1008,1009};
		zU32 puValueStatic[10] = {2000,2001,2002,2003,2004,2005,2006,2007,2008,2009};

		aDynamicA.resize(40);
		aStaticA.resize(40);
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
		for(zUInt idx(0), count(uTestSort.size()); idx<count; ++idx )
			zenIO::Printf(zenConst::keLog_Game, " %03i", uTestSort[idx] );

		uTestSort.Sort();
		zUInt uPrevValue(0);
		zenIO::Log(zenConst::keLog_Game, "Sorted    :");
		for(zUInt idx(0), count(uTestSort.size()); idx<count; ++idx )
		{
			zenAssert(	(uPrevValue < uTestSort[idx]) ||					//Previous value always smaller than current
						(uPrevValue == uTestSort[idx] && uPrevValue==2));	//Or current is equal to previous when it's 2 (only double inserted in array)
			zenIO::Printf(zenConst::keLog_Game, " %03i", uTestSort[idx] );
			uPrevValue = uTestSort[idx];
		}
#endif
	}

}