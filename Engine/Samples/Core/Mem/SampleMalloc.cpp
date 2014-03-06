#include "libZenEngine.h"

//=================================================================================================
//! @example SampleHash.cpp
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
			CMgr::Log.Log(CLog::keLog_Game, "Constructor");
		}
		
		TestAlloc(int value)
		: mValue(value)
		{			
			CMgr::Log.Log(CLog::keLog_Game, "Constructor %i", mValue);
		}
		
		~TestAlloc()
		{
			CMgr::Log.Log(CLog::keLog_Game, "Destructor %i", mValue);
		}
		int mValue;
	};


	//==================================================================================================
	//! @brief		Test simple new/malloc memory allocation	
	//==================================================================================================
	void SampleMalloc()
	{	
		CMgr::Log.Log(CLog::keLog_Game, zenConst::kzLineA40);
		CMgr::Log.Log(CLog::keLog_Game, " MALLOC");
		CMgr::Log.Log(CLog::keLog_Game, zenConst::kzLineA40);

		zenU32 size1 = sizeof(int);
		zenU32 size2 = sizeof(TestAlloc);

		TestAlloc* pTest1		= zenNewDefault TestAlloc(1);
		TestAlloc* pTest2		= zenNewDefault TestAlloc(2);
		TestAlloc* pTest3		= zenNew(&zenMem::Allocator::GetDefault()) TestAlloc(3);
		TestAlloc* pArrayTest	= zenNewDefault TestAlloc[5];

		//Test buffer overrun detection
		//pTest1[1].mValue = 5;
		delete pTest3;
		delete pTest2;
		delete pTest1;
		delete[] pArrayTest;

		zenMem::AllocatorPool PoolAlloc( "TestPool", sizeof(TestAlloc), 2, 1); 
		pTest1 = zenNew(&PoolAlloc) TestAlloc(0x01);
		pTest2 = zenNew(&PoolAlloc) TestAlloc(0x02);
		pTest3 = zenNew(&PoolAlloc) TestAlloc(0x03);
		delete pTest3;
		delete pTest2;
		delete pTest1;

		pTest1 = zenNew(&PoolAlloc) TestAlloc(0x11);
		pTest2 = zenNew(&PoolAlloc) TestAlloc(0x12);
		pTest3 = zenNew(&PoolAlloc) TestAlloc(0x13);
		delete pTest3;
		delete pTest2;
		delete pTest1;

	//	TestAlloc* pTestMismatch1 = new TestAlloc(1);
//		delete[] pTestMismatch1;
	//	TestAlloc* pTestMismatch2 = new TestAlloc[5];
//		delete pTestMismatch2;
	}

}