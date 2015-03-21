#include "zenEngine.h"
#include <iostream> //for cout

//=================================================================================================
//! @example SampleSignal.cpp 
//! Tests and metrics on Signals/Slots system
//=================================================================================================

namespace sample
{

class sampleSlot0 : public zenSig::zSlot
{
public:
	sampleSlot0(const char* _zName)
	: mzDisplayName(_zName)
	{
	}

	static void EventReceived(void* _pObject)
	{
		sampleSlot0* pObject	= static_cast<sampleSlot0*>(_pObject);
		std::cout << "\n" << (const char*)pObject->mzDisplayName << " : Event triggered";
	}
protected:
	zString mzDisplayName;
};

class PerfSlotCallback0 : public zList2xNode
{
public:
	PerfSlotCallback0()
	: mpCallback( &PerfSlotCallback0::EventCallback )
	{
	}
	static void EventCallback()
	{
		static zUInt sCounter = 0;
		++sCounter;
	}
	void (*mpCallback)();
};

class TestSlotConnection : public zenSig::zSlot
{
public:
	TestSlotConnection()
	: mCounter(0)
	{
	}

	void EventReceived() 
	{ 
		++mCounter;	
	}

	void EventReceived(int val)
	{
		mCounter += val;
	}

	void EventReceived(float val1, float val2)
	{
		mCounter += zUInt(val1 + val2);
	}

	void EventReceived(float val1, float val2, float val3)
	{
		mCounter += zUInt(val1 + val2 + val3);
	}

	zUInt mCounter;
};

class TestAsyncSlotConnection : public zenSig::zSlot
{
public:
	TestAsyncSlotConnection()
	{
	}

	void EventReceived()
	{
		printf("\n Event");
	}

	void EventReceived(int val1, int val2)
	{
		printf("\n Event int2   (%4i, %4i)", val1, val2);
	}

	void EventReceived(float val1, float val2)
	{
		printf("\n Event float2 (%4.0f, %4.0f)", val1, val2);
	}
};

void SampleSignal()
{
	zenIO::Log(zenConst::keLog_Game, zenConst::kzLineA40);
	zenIO::Log(zenConst::keLog_Game, " SIGNALS");
	zenIO::Log(zenConst::keLog_Game, zenConst::kzLineA40);
	
	const zUInt kLoopCount	= 10000;	
	//--------------------------------------------------------
	// Callback
	//--------------------------------------------------------		
	PerfSlotCallback0	Callback0;
	zList2x				lstListener0;
	lstListener0.AddHead(&Callback0);

	zU64 uCallbackTime		= zenSys::GetTimeUSec();
	for (zU32 i = 0; i <kLoopCount; ++i)
	{
		zList2xNode* pListenerCur	= lstListener0.GetHead();
		const zList2xNode* pInvalid = lstListener0.GetInvalid();
		while( pListenerCur != pInvalid )
		{
			PerfSlotCallback0* pListener	= static_cast<PerfSlotCallback0*>(pListenerCur);
			pListenerCur					= pListenerCur->LstNext();
			pListener->mpCallback();
		}
	}
	uCallbackTime			= zenSys::GetTimeUSec() - uCallbackTime;
	zenIO::Printf(zenConst::keLog_Game, "\n\n Invocation time for %ix (Result+=1 per call) :", kLoopCount);
	zenIO::Printf(zenConst::keLog_Game, "\n    Callback       :%6.02fms", float(uCallbackTime) / 1000.0);

	TestSlotConnection		receiverSync;
	TestSlotConnection		receiverAsync;
	//--------------------------------------------------------
	// Signal - No Parameters
	//--------------------------------------------------------
	zenSig::zSignalEmitter0	sigNoParam;
	sigNoParam.Connect(receiverSync, &TestSlotConnection::EventReceived);	
	receiverSync.mCounter	= 0;
	receiverAsync.mCounter	= 0;
	zU64 uSignalTime		= zenSys::GetTimeUSec();
	for (zU32 i = 0; i < kLoopCount; ++i)
		sigNoParam.Emit();
	uSignalTime	= zenSys::GetTimeUSec() - uSignalTime;
	zenIO::Printf(zenConst::keLog_Game, "\n    Signal         :%6.02fms Result: %i", float(uSignalTime) / 1000.0, receiverSync.mCounter);

	sigNoParam.Disconnect(receiverSync);	
	bool bHasListeners	= sigNoParam.HasListeners();
	

	//--------------------------------------------------------
	// Signal - 1 Int parameters
	//--------------------------------------------------------
	zenSig::zSignalEmitter1<int> sigParam1;	
	sigParam1.Connect(receiverSync, &TestSlotConnection::EventReceived);
	receiverSync.mCounter	= 0;
	receiverAsync.mCounter	= 0;
	uSignalTime				= zenSys::GetTimeUSec();
	for (zU32 i = 0; i < kLoopCount; ++i)
		sigParam1.Emit(1);
	uSignalTime = zenSys::GetTimeUSec() - uSignalTime;
	zenIO::Printf(zenConst::keLog_Game, "\n    Signal 1       :%6.02fms Result: %i", float(uSignalTime) / 1000.0, receiverSync.mCounter);

	//--------------------------------------------------------
	// Signal - 2 float, float parameters
	//--------------------------------------------------------
	zenSig::zSignalEmitter2<float, float> sigParam2;
	sigParam2.Connect(receiverSync, &TestSlotConnection::EventReceived);
	receiverSync.mCounter	= 0;
	receiverAsync.mCounter	= 0;
	uSignalTime				= zenSys::GetTimeUSec();
	for (zU32 i = 0; i < kLoopCount; ++i)
		sigParam2.Emit(0.5, 0.5);
	uSignalTime = zenSys::GetTimeUSec() - uSignalTime;
	zenIO::Printf(zenConst::keLog_Game, "\n    Signal 2       :%6.02fms Result: %i", float(uSignalTime) / 1000.0, receiverSync.mCounter);

	//--------------------------------------------------------
	// Signal - 3 float, float, float parameters
	//--------------------------------------------------------
	zenSig::zSignalEmitter3<float, float, float> sigParam3;
	sigParam3.Connect(receiverSync, &TestSlotConnection::EventReceived);
	receiverSync.mCounter	= 0;
	receiverAsync.mCounter	= 0;
	uSignalTime				= zenSys::GetTimeUSec();
	for (zU32 i = 0; i < kLoopCount; ++i)
		sigParam3.Emit(0.35f, 0.35f, 0.35f);
	uSignalTime = zenSys::GetTimeUSec() - uSignalTime;
	zenIO::Printf(zenConst::keLog_Game, "\n    Signal 3       :%6.02fms Result: %i", float(uSignalTime) / 1000.0, receiverSync.mCounter);

	//--------------------------------------------------------
	// Signal Async - No Parameters
	//--------------------------------------------------------
	zenSig::zSignalEmitterAsyncGroup	EmitterOwner;
	zenSig::zSignalEmitterAsync1<int>	SigParam1Async(&EmitterOwner);
	SigParam1Async.Connect(receiverAsync, &TestSlotConnection::EventReceived);
	receiverSync.mCounter	= 0;
	receiverAsync.mCounter	= 0;
	uSignalTime = zenSys::GetTimeUSec();
	for (zU32 i = 0; i < kLoopCount; ++i)
		SigParam1Async.Emit(1);
	zU64 uStoringTime = zenSys::GetTimeUSec() - uSignalTime;
	EmitterOwner.EmitAllSignals();
	uSignalTime = zenSys::GetTimeUSec() - uSignalTime;
	zenIO::Printf(zenConst::keLog_Game, "\n    Signal Async 1 :%6.02fms (%6.02fms storing) Result: %i", float(uSignalTime) / 1000.0, float(uStoringTime) / 1000.0, receiverAsync.mCounter);

	zenSig::zSignalEmitterAsync2<int, int> 		SigEmitterA(&EmitterOwner);
	zenSig::zSignalEmitterAsync2<float, float>	SigEmitterB(&EmitterOwner);
	TestAsyncSlotConnection						receiverAsync2;
	SigEmitterA.Connect(receiverAsync2, &TestAsyncSlotConnection::EventReceived);
	SigEmitterB.Connect(receiverAsync2, &TestAsyncSlotConnection::EventReceived);

	SigEmitterA.Emit(1, 1);
	SigEmitterA.Emit(2, 1);
	SigEmitterB.Emit(3, 2);
	SigEmitterB.Emit(4, 2);
	SigEmitterA.Emit(5, 1);
	SigEmitterB.Emit(6, 2);
	EmitterOwner.EmitAllSignals();

	zenIO::Printf(zenConst::keLog_Game, "\n\n Signal Disconnect OK? : %i", bHasListeners == false );	

}

}