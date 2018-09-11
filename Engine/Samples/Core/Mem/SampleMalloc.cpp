#include "zenEngine.h"

//=================================================================================================
//! @example SampleMalloc.cpp
//! Informations on the usage of various string Hashing functionalities
//=================================================================================================
namespace sample
{
	class TestAlloc
	{
	public:
		TestAlloc()
		: mValue(0)
		{
			zenIO::Log(zenConst::keLog_Game, "Constructor");
		}
		
		TestAlloc(int value)
		: mValue(value)
		{			
			zenIO::Log(zenConst::keLog_Game, "Constructor %i", mValue);
		}
		
		~TestAlloc()
		{
			zenIO::Log(zenConst::keLog_Game, "Destructor %i", mValue);
		}
		int mValue;
	};

	struct Test
	{
		zUInt Value1 : 6;
		zUInt Value2 : 2;
	};
	struct Test2 : public Test
	{
		zUInt Value3 : 4;
		zUInt Value4 : 4;
	};

	struct TestPoolA { char Values[16*20]; };
	struct TestPoolB { char Values[16*22]; };

	struct TestArrayResize
	{
		TestArrayResize()
		{			
			static int i(0);
			mValue = i++;
		}
		virtual ~TestArrayResize()
		{
		}
		zU64 mValue = 0;
	};

	//==================================================================================================
	//! @brief		Test simple new/malloc memory allocation	
	//==================================================================================================
	void SampleMalloc()
	{	
		zenIO::Log(zenConst::keLog_Game, zenConst::kzLineA40);
		zenIO::Log(zenConst::keLog_Game, " MALLOC");
		zenIO::Log(zenConst::keLog_Game, zenConst::kzLineA40);

		TestArrayResize* pTestElement1 = zenNew TestArrayResize[10];
		delete[] pTestElement1;
		
		size_t count = 6000;
		TestArrayResize* pTestElement2 = zenNewWithResize(4000) TestArrayResize[10];
		pTestElement2[0].mValue		= 0;
		//pTestElement2[6000].mValue	= 0; // Crash
		zenMem::Resize(pTestElement2, 4000);
		pTestElement2[3999].mValue	= 0; // Ok

		//------------------------------------------------------------------------------------------
		// Testing Pool Allocation
		//------------------------------------------------------------------------------------------
		std::array<TestPoolA*,4> aPoolAllocA;
		for(auto& pItem : aPoolAllocA)
			pItem = zenNewPool TestPoolA();

		std::array<TestPoolB*,4> aPoolAllocB;
		for(auto& pItem : aPoolAllocB)
			pItem = zenNewPool TestPoolB();

		for(auto& pItem : aPoolAllocA)
			delete pItem;

		for(auto& pItem : aPoolAllocB)
			delete pItem;

		int* pvalues = zenNew int;
		*pvalues = 1;
		delete pvalues;
		pvalues = zenNew int[10];
		pvalues[0] = 2;
		delete[] pvalues;

		//------------------------------------------------------------------------------------------
		// Testing OutOfBound access
		//------------------------------------------------------------------------------------------
		zU8* pProtectTest = zenNewCheck zU8[5];
		pProtectTest[0] = 0;
		pProtectTest[1] = 0;
		pProtectTest[2] = 0;
		pProtectTest[3] = 0;
		pProtectTest[4] = 0;
		//pProtectTest[5] = 0;					// Should generate Protection Fault
		delete[] pProtectTest;
		//volatile zU8 value = pProtectTest[0];	// Should generate Protection Fault
	
		TestAlloc* pTest1		= zenNew TestAlloc(1);
		TestAlloc* pTest2		= zenNew TestAlloc(2);
		TestAlloc* pTest3		= zenNew TestAlloc(3);
		TestAlloc* pArrayTest	= zenNew TestAlloc[5];

		//Test buffer overrun detection
		//pTest1[1].mValue = 5;
		delete pTest3;
		delete pTest2;
		delete pTest1;
		delete[] pArrayTest;

		//zenMem::zAllocatorPool PoolAlloc( "TestPool", sizeof(TestAlloc), 2, 1); 
		pTest1 = zenNewPool TestAlloc(0x01);
		pTest2 = zenNewPool TestAlloc(0x02);
		pTest3 = zenNewPool TestAlloc(0x03);
		delete pTest3;
		delete pTest2;
		delete pTest1;

		pTest1 = zenNewPool TestAlloc(0x11);
		pTest2 = zenNewPool TestAlloc(0x12);
		pTest3 = zenNewPool TestAlloc(0x13);
		delete pTest3;
		delete pTest2;
		delete pTest1;

	//	TestAlloc* pTestMismatch1 = new TestAlloc(1);
//		delete[] pTestMismatch1;
	//	TestAlloc* pTestMismatch2 = new TestAlloc[5];
//		delete pTestMismatch2;
	}

}