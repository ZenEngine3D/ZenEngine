#include "zenEngine.h"

//=================================================================================================
//! @example SampleFastPool.cpp
//! 
//=================================================================================================

namespace sample
{

static bool GConstructorCalled	= false;
static bool GDestructorCalled	= false;

struct StructTestPool
{
	StructTestPool() 
	: value(-1)
	{
		GConstructorCalled = true;		
	};
	StructTestPool(int _Init) 
	: value(_Init)
	{	
		GConstructorCalled = true;	
	};
	~StructTestPool() 
	{		
		GDestructorCalled = true;
	};
	int value;
};

class ClassPoolSpeedTestBase
{
public:
	ClassPoolSpeedTestBase() : value(0){};
	ClassPoolSpeedTestBase(int _value) : value(_value){};
	virtual ~ClassPoolSpeedTestBase(){};
	virtual int GetValue(){return value;}
	int value;
};

class ClassPoolSpeed : public ClassPoolSpeedTestBase
{
public:
	ClassPoolSpeed() : ClassPoolSpeedTestBase(){}
	ClassPoolSpeed(int _value) : ClassPoolSpeedTestBase(_value){};
	virtual int GetValue(){return value*2;}
};

void OutputObjectResult( bool _bSupportConstr, bool _bSupportDest )
{
	if( GConstructorCalled )zenIO::Log(zenConst::keLog_Game, " Constructor called valid       : %s", zenConst::kzFalseTrue[_bSupportConstr==GConstructorCalled]);
	else					zenIO::Log(zenConst::keLog_Game, " Constructor not called valid   : %s", zenConst::kzFalseTrue[_bSupportConstr==GConstructorCalled]);
	if( GDestructorCalled )	zenIO::Log(zenConst::keLog_Game, " Destructor called valid        : %s", zenConst::kzFalseTrue[_bSupportDest==GDestructorCalled]);
	else					zenIO::Log(zenConst::keLog_Game, " Destructor not called valid    : %s", zenConst::kzFalseTrue[_bSupportDest==GDestructorCalled]);							
}

//==================================================================================================
//! @brief		Test simple new/malloc memory allocation	
//==================================================================================================
void SampleFastPool()
{	
	zenIO::Log(zenConst::keLog_Game, "\n\n");
	zenIO::Log(zenConst::keLog_Game, zenConst::kzLineA40);
	zenIO::Log(zenConst::keLog_Game, " FASTPOOL");
	zenIO::Log(zenConst::keLog_Game, zenConst::kzLineA40);

	zenMem::zAllocatorFastPool<StructTestPool, false, false>	PoolAlloc(2,16);
	zenMem::zAllocatorFastPool<StructTestPool, true, false>		PoolAllocConstr(2,16);
	zenMem::zAllocatorFastPool<StructTestPool, false, true>		PoolAllocDest(2,16);
	// Testing basic allocation, pool expansion, deallocating and reallocating
	StructTestPool* pValues[5];
	zenMem::Set( pValues, 0, sizeof(pValues) );
	
	for(zU32 idx(0); idx<zenArrayCount(pValues); idx++)
	{
		pValues[idx]		= PoolAlloc.Allocate();
		pValues[idx]->value = idx;
	}
		
	PoolAlloc.Deallocate(pValues[0]);
	pValues[0]			= PoolAlloc.Allocate();
	pValues[0]->value	= 10;
	for(zUInt idx(0); idx<zenArrayCount(pValues); idx++)
		PoolAlloc.Deallocate(pValues[idx]);

	// Testing Constructor/Destructor call	
	GDestructorCalled			= false;
	GConstructorCalled			= false;
	StructTestPool* pTestVal	= PoolAllocConstr.Allocate();	
	PoolAllocConstr.Deallocate(pTestVal);
	OutputObjectResult(true, false);
	
	GDestructorCalled	= false;
	GConstructorCalled	= false;
	pTestVal = PoolAllocConstr.Allocate(10);
	zenIO::Log(zenConst::keLog_Game, " Constructor value assign valid : %s", zenConst::kzFalseTrue[pTestVal->value == 10] );	
	PoolAllocConstr.Deallocate(pTestVal);
	
	GDestructorCalled	= false;
	GConstructorCalled	= false;
	pTestVal			= PoolAllocDest.Allocate();	
	PoolAllocDest.Deallocate(pTestVal);
	OutputObjectResult(false, true);

	//----------------------------------------------------------------------------

	#define kuLoopCount 100000
	zenIO::Log(zenConst::keLog_Game, " \n Allocations Performances for %i struc       [Total, Allocate, Free]", kuLoopCount);
	zenMem::zAllocatorPool										PoolStandard("sample FastPool", sizeof(StructTestPool), 256, 256);
	zenMem::zAllocatorFastPool<StructTestPool, false, false>	PoolFast(256,256);
	zenMem::zAllocatorFastPool<StructTestPool, true, false>		PoolFastConst(256,256);
	zenMem::zAllocatorFastPool<StructTestPool, false, true>		PoolFastDest(256,256);
	zenMem::zAllocatorFastPool<StructTestPool, true, true>		PoolFastConstDest(256,256);
	StructTestPool**											pStructAlloc = zenNewDefault StructTestPool*[kuLoopCount];	

	zU64 uTimeAlloc, uTimeDealloc;
	
	uTimeAlloc		= zenSys::GetTimeUSec();
	for (zU32 i = 0; i < kuLoopCount; ++i)
		pStructAlloc[i] = zenNew(&PoolStandard) StructTestPool(i);
	uTimeAlloc		= zenSys::GetTimeUSec() - uTimeAlloc;
	uTimeDealloc	= zenSys::GetTimeUSec();
	for (zU32 i = 0; i < kuLoopCount; ++i)
		zenDel(pStructAlloc[i]);
	uTimeDealloc	= zenSys::GetTimeUSec() - uTimeDealloc;
	zenIO::Log(zenConst::keLog_Game, " Standard Pool      : [%5.02fms, %5.02fms, %5.02fms]", float(uTimeAlloc+uTimeDealloc)/1000.f,  float(uTimeAlloc)/1000.f, float(uTimeDealloc)/1000.f);

	uTimeAlloc		= zenSys::GetTimeUSec();
	for (zU32 i = 0; i < kuLoopCount; ++i)
		pStructAlloc[i] = PoolFast.Allocate();
	uTimeAlloc		= zenSys::GetTimeUSec() - uTimeAlloc;
	uTimeDealloc	= zenSys::GetTimeUSec();
	for (zU32 i = 0; i < kuLoopCount; ++i)
		PoolFast.Deallocate(pStructAlloc[i]);
	uTimeDealloc	= zenSys::GetTimeUSec() - uTimeDealloc;
	zenIO::Log(zenConst::keLog_Game, " Fast Pool          : [%5.02fms, %5.02fms, %5.02fms]", float(uTimeAlloc+uTimeDealloc)/1000.f,  float(uTimeAlloc)/1000.f, float(uTimeDealloc)/1000.f);

	uTimeAlloc		= zenSys::GetTimeUSec();
	for (zU32 i = 0; i < kuLoopCount; ++i)
		pStructAlloc[i] = PoolFastConst.Allocate(i);
	uTimeAlloc		= zenSys::GetTimeUSec() - uTimeAlloc;
	uTimeDealloc	= zenSys::GetTimeUSec();
	for (zU32 i = 0; i < kuLoopCount; ++i)
		PoolFastConst.Deallocate(pStructAlloc[i]);
	uTimeDealloc	= zenSys::GetTimeUSec() - uTimeDealloc;
	zenIO::Log(zenConst::keLog_Game, " Fast Pool w/Constr : [%5.02fms, %5.02fms, %5.02fms]", float(uTimeAlloc+uTimeDealloc)/1000.f,  float(uTimeAlloc)/1000.f, float(uTimeDealloc)/1000.f);

	uTimeAlloc		= zenSys::GetTimeUSec();
	for (zU32 i = 0; i < kuLoopCount; ++i)
		pStructAlloc[i] = PoolFastDest.Allocate();
	uTimeAlloc		= zenSys::GetTimeUSec() - uTimeAlloc;
	uTimeDealloc	= zenSys::GetTimeUSec();
	for (zU32 i = 0; i < kuLoopCount; ++i)
		PoolFastDest.Deallocate(pStructAlloc[i]);
	uTimeDealloc	= zenSys::GetTimeUSec() - uTimeDealloc;
	zenIO::Log(zenConst::keLog_Game, " Fast Pool w/Destr  : [%5.02fms, %5.02fms, %5.02fms]", float(uTimeAlloc+uTimeDealloc)/1000.f,  float(uTimeAlloc)/1000.f, float(uTimeDealloc)/1000.f);

	zenDelArray(pStructAlloc);

	//----------------------------------------------------------------------------
	zenIO::Log(zenConst::keLog_Game, " \n Allocations Performances for %i Class with virtual [Total, Allocate, Free]", kuLoopCount);
	zenMem::zAllocatorPool										PoolClassStandard("sample FastPool", sizeof(ClassPoolSpeed), 256, 256);
	zenMem::zAllocatorFastPool<ClassPoolSpeed, false, false>	PoolClassFast(256,256);
	zenMem::zAllocatorFastPool<ClassPoolSpeed, true, false>		PoolClassFastConst(256,256);
	zenMem::zAllocatorFastPool<ClassPoolSpeed, false, true>		PoolClassFastDest(256,256);
	zenMem::zAllocatorFastPool<ClassPoolSpeed, true, true>		PoolClassFastConstDest(256,256);
	ClassPoolSpeed**											pClassAlloc = zenNewDefault ClassPoolSpeed*[kuLoopCount];		

	uTimeAlloc		= zenSys::GetTimeUSec();
	for (zU32 i = 0; i < kuLoopCount; ++i)
		pClassAlloc[i] = zenNew(&PoolClassStandard) ClassPoolSpeed(i);
	uTimeAlloc		= zenSys::GetTimeUSec() - uTimeAlloc;
	uTimeDealloc	= zenSys::GetTimeUSec();
	for (zU32 i = 0; i < kuLoopCount; ++i)
		zenDel(pClassAlloc[i]);
	uTimeDealloc	= zenSys::GetTimeUSec() - uTimeDealloc;
	zenIO::Log(zenConst::keLog_Game, " Standard Pool         : [%5.02fms, %5.02fms, %5.02fms]", float(uTimeAlloc+uTimeDealloc)/1000.f,  float(uTimeAlloc)/1000.f, float(uTimeDealloc)/1000.f);
	
	uTimeAlloc		= zenSys::GetTimeUSec();
	for (zU32 i = 0; i < kuLoopCount; ++i)
		pClassAlloc[i] = PoolClassFast.Allocate();
	uTimeAlloc		= zenSys::GetTimeUSec() - uTimeAlloc;
	uTimeDealloc	= zenSys::GetTimeUSec();
	for (zU32 i = 0; i < kuLoopCount; ++i)
		PoolClassFast.Deallocate(pClassAlloc[i]);
	uTimeDealloc	= zenSys::GetTimeUSec() - uTimeDealloc;
	zenIO::Log(zenConst::keLog_Game, " Fast Pool             : [%5.02fms, %5.02fms, %5.02fms]", float(uTimeAlloc+uTimeDealloc)/1000.f,  float(uTimeAlloc)/1000.f, float(uTimeDealloc)/1000.f);
	
	uTimeAlloc		= zenSys::GetTimeUSec();
	for (zU32 i = 0; i < kuLoopCount; ++i)
		pClassAlloc[i] = PoolClassFastConst.Allocate(i);
	uTimeAlloc		= zenSys::GetTimeUSec() - uTimeAlloc;
	uTimeDealloc	= zenSys::GetTimeUSec();
	for (zU32 i = 0; i < kuLoopCount; ++i)
		PoolClassFastConst.Deallocate(pClassAlloc[i]);
	uTimeDealloc	= zenSys::GetTimeUSec() - uTimeDealloc;
	zenIO::Log(zenConst::keLog_Game, " Fast Pool w/Constr    : [%5.02fms, %5.02fms, %5.02fms]", float(uTimeAlloc+uTimeDealloc)/1000.f,  float(uTimeAlloc)/1000.f, float(uTimeDealloc)/1000.f);
	
	uTimeAlloc		= zenSys::GetTimeUSec();
	for (zU32 i = 0; i < kuLoopCount; ++i)
		pClassAlloc[i] = PoolClassFastConstDest.Allocate();
	uTimeAlloc		= zenSys::GetTimeUSec() - uTimeAlloc;
	uTimeDealloc	= zenSys::GetTimeUSec();
	for (zU32 i = 0; i < kuLoopCount; ++i)
		PoolClassFastConstDest.Deallocate(pClassAlloc[i]);
	uTimeDealloc	= zenSys::GetTimeUSec() - uTimeDealloc;
	zenIO::Log(zenConst::keLog_Game, " Fast Pool Constr&Destr: [%5.02fms, %5.02fms, %5.02fms]", float(uTimeAlloc+uTimeDealloc)/1000.f,  float(uTimeAlloc)/1000.f, float(uTimeDealloc)/1000.f);
	
	zenDelArray(pClassAlloc);
}

}