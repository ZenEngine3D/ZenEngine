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
		CMgr::Log.Log(CLog::keLog_Game, awconst::kzLineA40);
		CMgr::Log.Log(CLog::keLog_Game, " MALLOC");
		CMgr::Log.Log(CLog::keLog_Game, awconst::kzLineA40);

		awU32 size1 = sizeof(int);
		awU32 size2 = sizeof(TestAlloc);

		TestAlloc* pTest1		= AWNewDefault TestAlloc(1);
		TestAlloc* pTest2		= AWNewDefault TestAlloc(2);
		TestAlloc* pTest3		= AWNew(CMem::Allocator::Default) TestAlloc(3);
		TestAlloc* pArrayTest	= AWNewDefault TestAlloc[5];

		//Test buffer overrun detection
		//pTest1[1].mValue = 5;
		delete pTest3;
		delete pTest2;
		delete pTest1;
		delete[] pArrayTest;

		CMem::PoolAllocator PoolAlloc( "TestPool", sizeof(TestAlloc), 2, 1); 
		pTest1 = AWNew(&PoolAlloc) TestAlloc(0x01);
		pTest2 = AWNew(&PoolAlloc) TestAlloc(0x02);
		pTest3 = AWNew(&PoolAlloc) TestAlloc(0x03);
		delete pTest3;
		delete pTest2;
		delete pTest1;

		pTest1 = AWNew(&PoolAlloc) TestAlloc(0x11);
		pTest2 = AWNew(&PoolAlloc) TestAlloc(0x12);
		pTest3 = AWNew(&PoolAlloc) TestAlloc(0x13);
		delete pTest3;
		delete pTest2;
		delete pTest1;

	//	TestAlloc* pTestMismatch1 = new TestAlloc(1);
//		delete[] pTestMismatch1;
	//	TestAlloc* pTestMismatch2 = new TestAlloc[5];
//		delete pTestMismatch2;
	}

}