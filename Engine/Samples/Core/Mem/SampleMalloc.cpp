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
	//! @brief		Test simple new/malloc memory allocation	
	//==================================================================================================
	void SampleMalloc()
	{	
		zU32* pIntVal[6]; ClassTestResize* pClassVal[6];
		pIntVal[0]		= zenMem::New<zU32>();
		pIntVal[1]		= zenMem::New<zU32>(1);
		pIntVal[2]		= zenMem::NewPool<zU32>(2);
		pIntVal[3]		= zenMem::NewArray<zU32>(10, 3);
		pIntVal[4]		= zenMem::NewArray<zU32>(10);
		pIntVal[5]		= zenMem::NewResizeable<zU32>(10, 1);
		pClassVal[0]	= zenMem::New<ClassTestResize>();
		pClassVal[1]	= zenMem::New<ClassTestResize>(1,2);
		pClassVal[2]	= zenMem::NewPool<ClassTestResize>(3,4);
		pClassVal[3]	= zenMem::NewArray<ClassTestResize>(10);
		pClassVal[4]	= zenMem::NewArray<ClassTestResize>(10, 5, 6);
		pClassVal[5]	= zenMem::NewResizeable<ClassTestResize>(10, 5, 5);
		
		pIntVal[5]		= zenMem::Resize(pIntVal[5], 20);
		pIntVal[5]		= zenMem::Resize(pIntVal[5], 5);

		pClassVal[5]	= zenMem::Resize(pClassVal[5], 20);
		pClassVal[5]	= zenMem::Resize(pClassVal[5], 5);

		for(int i(0); i<zenArrayCount(pIntVal); ++i)
			zenMem::Del(pIntVal[i]);
		for(int i(0); i<zenArrayCount(pClassVal); ++i)
			zenMem::Del(pClassVal[i]);


		zenIO::Log(zenConst::keLog_Game, zenConst::kzLineA40);
		zenIO::Log(zenConst::keLog_Game, " MALLOC");
		zenIO::Log(zenConst::keLog_Game, zenConst::kzLineA40);

		TestArrayResize* pTestElement1 = zenMem::NewArray<TestArrayResize>(10);
		zenMem::Del(pTestElement1);
		
		size_t count = 6000;
		TestArrayResize* pTestElement2 = zenMem::NewResizeable<TestArrayResize>(10);
		pTestElement2[0].mValue		= 0;
		//pTestElement2[6000].mValue	= 0; // Crash
		pTestElement2 = zenMem::Resize(pTestElement2, 4000);
		pTestElement2[3999].mValue	= 0; // Ok

		//------------------------------------------------------------------------------------------
		// Testing Pool Allocation
		//------------------------------------------------------------------------------------------
		std::array<TestPoolA*,4> aPoolAllocA;
		for(auto& pItem : aPoolAllocA)
			pItem = zenMem::NewPool<TestPoolA>();

		std::array<TestPoolB*,4> aPoolAllocB;
		for(auto& pItem : aPoolAllocB)
			pItem = zenMem::NewPool<TestPoolB>();

		for(auto& pItem : aPoolAllocA)
			zenMem::Del(pItem);

		for(auto& pItem : aPoolAllocB)
			zenMem::Del(pItem);

		int* pvalues = zenMem::New<int>();
		*pvalues = 1;
		zenMem::Del(pvalues);
		pvalues = zenMem::NewArray<int>(10);
		pvalues[0] = 2;
		zenMem::Del(pvalues);

		//------------------------------------------------------------------------------------------
		// Testing OutOfBound access
		//------------------------------------------------------------------------------------------
		zU8* pProtectTest = zenMem::NewProtected<zU8>(5);
		pProtectTest[0] = 0;
		pProtectTest[1] = 0;
		pProtectTest[2] = 0;
		pProtectTest[3] = 0;
		pProtectTest[4] = 0;
		//pProtectTest[5] = 0;					// Should generate Protection Fault
		zenMem::Del(pProtectTest);
		//volatile zU8 value = pProtectTest[0];	// Should generate Protection Fault
	
		TestAlloc* pTest1		= zenMem::New<TestAlloc>(1);
		TestAlloc* pTest2		= zenMem::New<TestAlloc>(2);
		TestAlloc* pTest3		= zenMem::New<TestAlloc>(3);
		TestAlloc* pArrayTest	= zenMem::NewArray<TestAlloc>(5);

		//Test buffer overrun detection
		//pTest1[1].mValue = 5;
		zenMem::Del(pTest3);
		zenMem::Del(pTest2);
		zenMem::Del(pTest1);
		zenMem::Del(pArrayTest);

		pTest1 = zenMem::NewPool<TestAlloc>(0x01);
		pTest2 = zenMem::NewPool<TestAlloc>(0x02);
		pTest3 = zenMem::NewPool<TestAlloc>(0x03);
		zenMem::Del(pTest3);
		zenMem::Del(pTest2);
		zenMem::Del(pTest1);

		pTest1 = zenMem::NewPool<TestAlloc>(0x11);
		pTest2 = zenMem::NewPool<TestAlloc>(0x12);
		pTest3 = zenMem::NewPool<TestAlloc>(0x13);
		zenMem::Del(pTest3);
		zenMem::Del(pTest2);
		zenMem::Del(pTest1);

	//	TestAlloc* pTestMismatch1 = new TestAlloc(1);
//		zenMem::Del(pTestMismatch1);
	//	TestAlloc* pTestMismatch2 = new TestAlloc[5];
//		zenMem::Del(pTestMismatch2);
	}

}