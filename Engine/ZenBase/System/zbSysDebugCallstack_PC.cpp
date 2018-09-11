#include "zbBase.h"
#include <Dbghelp.h>	//For Stacktrace informations
#include <mutex>

namespace zbSys
{    

class CallstackInit : public Callstack
{
public:
	CallstackInit(zU32 _Depth) 
	: Callstack(_Depth)
	{
	}
	Callstack::Entry& GetEntry(zU32 _Depth)
	{
		zenAssert(_Depth < mStackDepth)
		return maStackEntries[_Depth];
	}
	static size_t GetNeededSize(zU32 _StackDepth)
	{
		return sizeof(Callstack) + sizeof(Entry)*_StackDepth;
	}
};

class CallstackDepot
{
zenClassDeclareNoParent(CallstackDepot)
public:
								CallstackDepot();	
	const Callstack*			GetAddCallstack(zUInt _IgnoreTopCount);

protected:		
	const char*					GetAddStringCopy(const char* _String);
	zMap<const char*>::Key64	mdStrings;
	char*						mpStringMemory		= nullptr;	//!< Keeps copy of strings used for Callstack tracking
	size_t						mStringMemorySize	= 0;
	zMap<Callstack*>::Key64		mdCallstack;
	zU8*						mpCallstackMemory;
	zUInt						muCallstackMemorySize = 0;
	std::mutex					mMutex;
};

CallstackDepot::CallstackDepot()
{
	SymInitialize(GetCurrentProcess(), nullptr, true); 
	mpStringMemory		= zenNewWithResize(4*1024*1204) char[zenVariableSize];
	mpCallstackMemory	= zenNewWithResize(100000*CallstackInit::GetNeededSize(128)) zU8[zenVariableSize];
	mdCallstack.Init(1024);
	mdStrings.Init(1024);		
}

const char* CallstackDepot::GetAddStringCopy(const char* _String)
{
	const zHash64 stringKey(_String);
	const char*& pString = mdStrings.GetAdd(stringKey);
	if( pString == nullptr )
	{
		size_t len			= strlen(_String) + 1;
		char* pStringCopy	= &mpStringMemory[mStringMemorySize];
		pString				= pStringCopy;
		mStringMemorySize	+= len;
		zenMem::ResizeMemory(mpStringMemory, mStringMemorySize);
		strncpy_s(pStringCopy, len, _String, _TRUNCATE);		
	}
	return pString;
}	

const Callstack* CallstackDepot::GetAddCallstack(zUInt _IgnoreTopCount)
{
	std::array<DWORD64, 128>	StackPCOffset;
	zHash64						CallstackKey;
	zU32						StackDepth(0);

	// Set up stack frame.
	STACKFRAME64				StackFrame;
	CONTEXT						Context;
	RtlCaptureContext(&Context);
	ZeroMemory( &StackFrame, sizeof( STACKFRAME64 ) );
	StackFrame.AddrPC.Offset	= Context.Rip;
	StackFrame.AddrPC.Mode		= AddrModeFlat;
	StackFrame.AddrFrame.Offset	= Context.Rsp;
	StackFrame.AddrFrame.Mode	= AddrModeFlat;
	StackFrame.AddrStack.Offset	= Context.Rsp;
	StackFrame.AddrStack.Mode	= AddrModeFlat;			
	
	bool bValidStack	= true;
	auto CurrentProcess = GetCurrentProcess();
	auto CurrentThread	= GetCurrentThread();
	zInt IgnoreTopCount	= static_cast<zInt>(_IgnoreTopCount + 1); //Ignore this method in the stack trace
	while ( StackDepth < StackPCOffset.size() && bValidStack)
	{
		bValidStack	= StackWalk64(IMAGE_FILE_MACHINE_AMD64, CurrentProcess, CurrentThread, &StackFrame, &Context, nullptr, SymFunctionTableAccess64, SymGetModuleBase64, nullptr) > 0;
		bValidStack	&= StackFrame.AddrPC.Offset != 0;	// 0 when base is reached
		if ( bValidStack && IgnoreTopCount-- >= 0 )
		{
			StackPCOffset[ StackDepth++ ] = StackFrame.AddrPC.Offset;
			CallstackKey.Append(&StackFrame.AddrPC.Offset, sizeof(StackFrame.AddrPC.Offset));
		}
	}

	mMutex.lock();
	Callstack*& pCallstack = mdCallstack.GetAdd(CallstackKey);
	mMutex.unlock();

	if( pCallstack == nullptr )
	{		
		std::array<char, sizeof(IMAGEHLP_SYMBOL64)+1024> SymbolMemory;
		IMAGEHLP_SYMBOL64* pSymbol = reinterpret_cast<IMAGEHLP_SYMBOL64*>(&SymbolMemory[0]);
		IMAGEHLP_LINE64 Line;
		DWORD LineDisplace(0);
		Line.SizeOfStruct				= sizeof(IMAGEHLP_LINE64);
		pSymbol->MaxNameLength			= static_cast<DWORD>(SymbolMemory.size() - sizeof(IMAGEHLP_SYMBOL64));
		pSymbol->SizeOfStruct			= sizeof(SymbolMemory);

		zU8* pStackMemoryCur			= &mpCallstackMemory[muCallstackMemorySize];
		muCallstackMemorySize			+= CallstackInit::GetNeededSize(StackDepth);
		zenMem::ResizeMemory(mpCallstackMemory, muCallstackMemorySize);
		CallstackInit* pCallstackInit	= new(pStackMemoryCur) CallstackInit(StackDepth);
		pCallstack						= pCallstackInit;
		for(zU32 idxEntry(0); idxEntry<StackDepth; ++idxEntry)
		{
			Callstack::Entry& Entry = pCallstackInit->GetEntry(idxEntry);
			if( SymGetSymFromAddr64(CurrentProcess, StackPCOffset[idxEntry], nullptr, pSymbol) )
			{
				//size_t result = UnDecorateSymbolName(pSymbol->Name, Undecorated, sizeof(Undecorated), UNDNAME_COMPLETE);
				Entry.mFunctionName = GetAddStringCopy(pSymbol->Name);				
			}

			if( SymGetLineFromAddr64(CurrentProcess, StackPCOffset[idxEntry], &LineDisplace, &Line)  )
			{
				Entry.mFileName	= GetAddStringCopy(Line.FileName);
				Entry.mFileLine	= Line.LineNumber;
			}
		}
	}
	return pCallstack;
}

const Callstack* Callstack::GetAddCallstack(zUInt _IgnoreTopCount)
{
	thread_local bool sBlockRecursive(false);
	const Callstack* pCallstack(nullptr);
	if( sBlockRecursive == false )
	{
		sBlockRecursive = true; //During Initialization of Callstacks, 'new' will be called, creating an infinite recursive call to GetAddCallstack
		static CallstackDepot Callstacks;
		pCallstack = Callstacks.GetAddCallstack(_IgnoreTopCount+1);
		sBlockRecursive = false;
	}
	return pCallstack;
}

}  
