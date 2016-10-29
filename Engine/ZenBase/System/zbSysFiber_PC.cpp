#include "zbBase.h"
#include <thread>

namespace zbSys
{    

DWORD	gaThreadID[zbFiber::keThreadMax];
HANDLE	gaThreadHandle[zbFiber::keThreadMax];
LPVOID	gaLoadingFiber[zbFiber::keThreadMax];

VOID __stdcall FiberEntryPoint(LPVOID lpParameter)
{
	zbFiber* pFiber = static_cast<zbFiber*>(lpParameter);
	pFiber->InfiniteLoop();	
}

DWORD WINAPI StartThread(LPVOID lpParameter)
{
	zUInt Id			= (zUInt)lpParameter;
	gaLoadingFiber[Id]	= ConvertThreadToFiber(nullptr);	
	SetThreadAffinityMask(GetCurrentThread(), zU64(1)<<Id);
	FiberUpdateLoop(Id);
	return 0;
}

void StartTaskProcessing()
{
	zUInt maxStackSize(1024*16);	
	for(zUInt idx(0); idx<zbFiber::keFiberCount; ++idx)
		zenNewDefault zbFiber(maxStackSize);
			
	gaThreadHandle[0]	= GetCurrentThread();
	gaThreadID[0]		= GetCurrentThreadId();
	for(zUInt i(1), ThreadCount(zenSys::zTask::GetCPUCoreCount()); i<ThreadCount; ++i)
		gaThreadHandle[i] = CreateThread(nullptr, maxStackSize, StartThread, (LPVOID)i, 0 , gaThreadID+i);
		
	StartThread(0);
}

zbFiber_PC::zbFiber_PC(zUInt _StackSize)
: zbFiber_Base(_StackSize)
, mpFiber(nullptr)
{
	mpFiber = CreateFiber(_StackSize, FiberEntryPoint, this);
}

zbFiber_PC::~zbFiber_PC()
{
	if( mpFiber )
		DeleteFiber(mpFiber);
	mpFiber = nullptr;
}

void zbFiber_PC::HALActivate()
{	
	SwitchToFiber( mpFiber );
}

void zbFiber_PC::HALDeactivate()
{
	SwitchToFiber( gaLoadingFiber[muThreadId] );
}

}  
