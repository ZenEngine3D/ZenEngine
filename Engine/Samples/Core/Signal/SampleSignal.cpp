#include "zenEngine.h"
#include <iostream> //for cout

//=================================================================================================
//! @example SampleSignal.cpp 
//! Tests and metrics on Signals/Slots system
//=================================================================================================
namespace sample
{

//==================================================================================================
//! @class	ClassWithSlot
//! @detail	Basic example of objects receiving signal and processing them
//==================================================================================================
class ClassWithSlot
{
public:
	ClassWithSlot(const char* _zName)
	: mzDisplayName(_zName)
	{
	}

	//----------------------------------------------------------------------------------------------
	//! @brief Receive 'SignalA' callbacks, with no parameters
	//----------------------------------------------------------------------------------------------
	zenInline void ConnectSlotSignalA(zenSig::zSignal<>& InSignal)	
	{				
		mSlotSignalA.Connect(InSignal, [this]()
		{ 
			printf("\n %s : SignalA triggered, no parameters", mzDisplayName);
		} );
	}

	//----------------------------------------------------------------------------------------------
	//! @brief		Receive 'SignalB' callbacks, with 1 bool parameters
	//! @details	_bDisconnect parameter in signal is used to disconnect the slot when true
	//----------------------------------------------------------------------------------------------
	zenInline void ConnectSlotSignalB(zenSig::zSignal<bool>& InSignal)
	{		
		mSlotSignalB.Connect(InSignal, [this](bool _bDisconnect)
		{ 
			printf("\n %s : SignalB triggered, Disconnect=%s", mzDisplayName, _bDisconnect ? "True" : "False");
			if( _bDisconnect )
				mSlotSignalB.Disconnect();
		} );		
	}

protected:
	const char*					mzDisplayName;
	zenSig::zSignal<>::Slot		mSlotSignalA;
	zenSig::zSignal<bool>::Slot	mSlotSignalB;
};

//==================================================================================================
//! @class	ClassSlotPerfs
//! @detail	Class to test performances of processing signals instead of using function pointers
//==================================================================================================
class ClassSlotPerfs
{
public:
	//----------------------------------------------------------------------------------------------
	//! @brief Receive signals and process them with a 'Lambda'
	//----------------------------------------------------------------------------------------------
	zenInline void ConnectSlotLambda(zenSig::zSignal<int,int>& InSignal)
	{				
		mSlotLambda.Connect(InSignal, [this](int _Value1, int _Value2)
		{ 
			msSumLambda += _Value1 + _Value2;
		} );
	}

	//----------------------------------------------------------------------------------------------
	//! @brief Receive signals and process them with a 'Function callback'
	//----------------------------------------------------------------------------------------------
	zenInline void ConnectSlotFunction(zenSig::zSignal<int,int>& InSignal)
	{	
		mSlotFunction.Connect(InSignal, std::bind(&ClassSlotPerfs::CallbackFunction, this, std::placeholders::_1, std::placeholders::_2) );
	}

	int									msSumLambda		= 0;
	int									msSumFunction	= 0;

protected:
	void CallbackFunction(int _Value1, int _Value2)
	{
		msSumFunction += _Value1 + _Value2;
	}
	zenSig::zSignal<int,int>::Slot		mSlotLambda;
	zenSig::zSignal<int,int>::Slot		mSlotFunction;
};


void SlotCallbackSignalA()
{
	printf("\n %s : SignalA triggered, no parameters", "Function");
}

void SlotCallbackSignalB(bool _bDisconnect)
{
	printf("\n %s : SignalB triggered, Disconnect=%s (ignored)", "Function", _bDisconnect ? "True" : "False");
}

int gsCallbackSum(0);
void SlotCallbackAdd(int _Value1, int _Value2)
{
	gsCallbackSum += _Value1 + _Value2;
}


template<typename TClass, typename TMemberClass>
constexpr size_t zenOffsetOf2(TMemberClass TClass::*member)
{
	return (char*)&((TClass*)nullptr->*member) - (char*)nullptr;
}

template<class TItem, int TItem::* TLinkOffset>
struct TestList
{
	bool IsOk(TItem& Item, int& Value)  
	{
		//const size_t uVirtualTableOffset = TVirtual1stTime ? sizeof(size_t) : 0;
		int* PtrInt = (int*)(((char*)&Item) + zenOffsetOf2(TLinkOffset));
		bool bOk = PtrInt == &Value;
		//return zenOffsetOf2(TLinkOffset) + uVirtualTableOffset;
		return bOk;
	}	
};

template<class TItem, int TItem::* TLinkOffset>
constexpr size_t TestOffset()  
{
	return zenOffsetOf2(TLinkOffset);	
}

struct Base
{
	int value1;
	int value2;
};
struct BaseB : public Base
{
	int value3;
};

struct BaseV
{
	virtual ~BaseV(){};
	int value1;
	int value2;
};
struct BaseVB : public BaseV
{
	int value3;
};

void SampleSignal()
{
	zenIO::Log(zenConst::keLog_Game, zenConst::kzLineA60);
	zenIO::Log(zenConst::keLog_Game, " SIGNALS");
	zenIO::Log(zenConst::keLog_Game, zenConst::kzLineA60);

	//----------------------------------------------------------------------------------------------
	// Basic Signal/Slot setup with Objects/Lambda/Functions. 
	// Signals have no parameters
	//----------------------------------------------------------------------------------------------
	ClassWithSlot					ObjectWithSlot1("Object 1");
	ClassWithSlot					ObjectWithSlot2("Object 2");	
	zenSig::zSignal<>				SignalA;
	decltype(SignalA)::Slot	SlotSignalALambda;
	decltype(SignalA)::Slot	SlotSignalAFunction;

	ObjectWithSlot1.ConnectSlotSignalA(SignalA);					// Connect Object1 slot
	ObjectWithSlot2.ConnectSlotSignalA(SignalA);					// Connect Object2 slot
	SlotSignalAFunction.Connect(SignalA, SlotCallbackSignalA);	// Connect a function callback slot
	SlotSignalALambda.Connect(SignalA, []()						// Connect a Lambda slot
	{
		printf("\n %s : SignalA triggered, no parameters", "Lambda  ");
	});
	printf("\n%s\n Testing Signal Slot with no parameter\n%s", zenConst::kzLineA80, zenConst::kzLineA80);
	SignalA.Emit();											// Trigger Emitter A Signal (will be sent to all connected slots)

	//----------------------------------------------------------------------------------------------
	// Basic Signal/Slot setup with Objects/Lambda/Functions. 
	// Signals have 1 bool parameter
	//----------------------------------------------------------------------------------------------
	zenSig::zSignal<bool>			SignalB;
	decltype(SignalB)::Slot	SlotSignalBLambda;
	decltype(SignalB)::Slot	SlotSignalBFunction;

	ObjectWithSlot1.ConnectSlotSignalB(SignalB);					// Connect Object1 slot
	ObjectWithSlot2.ConnectSlotSignalB(SignalB);					// Connect Object2 slot
	SlotSignalBFunction.Connect(SignalB, SlotCallbackSignalB);		// Connect a function callback slot
	SlotSignalBLambda.Connect(SignalB, [](bool _bDisconnect)		// Connect a Lambda slot
	{
		printf("\n %s : SignalB triggered, Disconnect=%s (but ignored)", "Lambda  ", _bDisconnect ? "True " : "False");
	});
	
	
	printf("\n\n%s\n Testing Signal Slot with 1 bool parameter and disconnect support\n%s", zenConst::kzLineA80, zenConst::kzLineA80);
	SignalB.Emit(true);												// Trigger Emitter B Signal (will be sent to all connected slots)	
	printf("\n Only 'Function' and 'Lambda' slot connected next...");	
	SignalB.Emit(false);											// Trigger Emitter B Signal again, only 2 slot should respond now	

	//----------------------------------------------------------------------------------------------
	// Basic Signal/Slot setup with Objects/Lambda/Functions. 
	//----------------------------------------------------------------------------------------------
	const int kLoopCount	= 1000000;	
	printf("\n\n%s\n Performances of Signal/Slot versus function pointer (%i iterations)\n%s", zenConst::kzLineA80, kLoopCount, zenConst::kzLineA80);	
	//--------------------------------------------------------
	// Simple Function Pointer
	//--------------------------------------------------------
	{
		void (*pCallback)(int,int)	= SlotCallbackAdd;
		auto uFunctionPtrTimeMs		= zenSys::GetTimeUSec();
		gsCallbackSum				= 0;
		for(int i=0; i<kLoopCount; ++i)
		{
			(*pCallback)(1,1);
		}
		uFunctionPtrTimeMs			= zenSys::GetTimeUSec() - uFunctionPtrTimeMs;
		printf("\n Function Pointer , Sum Ok?%s(%i) Time:%ims", gsCallbackSum==kLoopCount*2 ? "True " : "False", (int)gsCallbackSum, (int)uFunctionPtrTimeMs/1000);
	}
	//--------------------------------------------------------
	// std::Function (used by Signal slots)
	//--------------------------------------------------------
	{
		std::function<void(int,int)> Callback	= SlotCallbackAdd;
		auto uFunctionPtrTimeMs					= zenSys::GetTimeUSec();
		gsCallbackSum							= 0;
		for(int i=0; i<kLoopCount; ++i)
		{
			Callback(1,1);
		}
		uFunctionPtrTimeMs			= zenSys::GetTimeUSec() - uFunctionPtrTimeMs;
		printf("\n Std::Function    , Sum Ok?%s(%i) Time:%ims", gsCallbackSum==kLoopCount*2 ? "True " : "False", (int)gsCallbackSum, (int)uFunctionPtrTimeMs/1000);
	}
	//--------------------------------------------------------
	// Signal with Function callback
	//--------------------------------------------------------
	zenSig::zSignal<int,int> SignalPerf;
	{
		decltype(SignalPerf)::Slot	SlotPerf;					// Will be automatically remove once out of scope
		SlotPerf.Connect(SignalPerf, SlotCallbackAdd);			// Connect a function callback slot
		auto uFunctionPtrTimeMs		= zenSys::GetTimeUSec();
		gsCallbackSum				= 0;
		for(int i=0; i<kLoopCount; ++i)
		{
			SignalPerf.Emit(1,1);
		}
		uFunctionPtrTimeMs			= zenSys::GetTimeUSec() - uFunctionPtrTimeMs;
		printf("\n Signal Function  , Sum Ok?%s(%i) Time:%ims", gsCallbackSum==kLoopCount*2 ? "True " : "False", (int)gsCallbackSum, (int)uFunctionPtrTimeMs/1000);
	}
	//--------------------------------------------------------
	// Signal with Lambda callback
	//--------------------------------------------------------
	{
		decltype(SignalPerf)::Slot	SlotPerf;						// Will be automatically remove once out of scope
		SlotPerf.Connect(SignalPerf, [](int _Value1, int _Value2)	// Connect a Lambda slot
		{
			gsCallbackSum += _Value1 + _Value2;
		});
		auto uFunctionPtrTimeMs		= zenSys::GetTimeUSec();
		gsCallbackSum				= 0;
		for(int i=0; i<kLoopCount; ++i)
		{
			SignalPerf.Emit(1,1);
		}
		uFunctionPtrTimeMs			= zenSys::GetTimeUSec() - uFunctionPtrTimeMs;
		printf("\n Signal Lambda    , Sum Ok?%s(%i) Time:%ims", gsCallbackSum==kLoopCount*2 ? "True " : "False", (int)gsCallbackSum, (int)uFunctionPtrTimeMs/1000);
	}
	//--------------------------------------------------------
	// Signal with Object Function callback
	//--------------------------------------------------------
	{
		decltype(SignalPerf)::Slot	SlotPerf;						// Will be automatically remove once out of scope
		ClassSlotPerfs				ObjectWithSlot;
		ObjectWithSlot.ConnectSlotFunction(SignalPerf);
		auto uFunctionPtrTimeMs		= zenSys::GetTimeUSec();
		for(int i=0; i<kLoopCount; ++i)
		{
			SignalPerf.Emit(1,1);
		}
		uFunctionPtrTimeMs			= zenSys::GetTimeUSec() - uFunctionPtrTimeMs;
		printf("\n Obj with Function, Sum Ok?%s(%i) Time:%ims", ObjectWithSlot.msSumFunction==kLoopCount*2 ? "True " : "False", (int)ObjectWithSlot.msSumFunction, (int)uFunctionPtrTimeMs/1000);
	}
	//--------------------------------------------------------
	// Signal with Object Lambda callback
	//--------------------------------------------------------
	{
		decltype(SignalPerf)::Slot	SlotPerf;						// Will be automatically remove once out of scope
		ClassSlotPerfs				ObjectWithSlot;
		ObjectWithSlot.ConnectSlotLambda(SignalPerf);
		auto uFunctionPtrTimeMs		= zenSys::GetTimeUSec();
		for(int i=0; i<kLoopCount; ++i)
		{
			SignalPerf.Emit(1,1);
		}
		uFunctionPtrTimeMs			= zenSys::GetTimeUSec() - uFunctionPtrTimeMs;
		printf("\n Obj with Lambda  , Sum Ok?%s(%i) Time:%ims", ObjectWithSlot.msSumLambda==kLoopCount*2 ? "True " : "False", (int)ObjectWithSlot.msSumLambda, (int)uFunctionPtrTimeMs/1000);
	}
}

}