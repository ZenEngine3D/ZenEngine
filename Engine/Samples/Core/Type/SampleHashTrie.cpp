#include "libZenEngine.h"
#include <stdio.h>//test
#include <stdlib.h>
#include <time.h>
#include <map>
#include <set>
#include <unordered_map>


namespace tracking_allocator
{
	size_t g_bytesAllocated = 0;
	size_t g_numAllocs = 0;

	template <class T> class allocator;

	// specialize for void:
	template <>
	class allocator<void>
	{
	public:
		typedef void*       pointer;
		typedef const void* const_pointer;
		// reference to void members are impossible.
		typedef void        value_type;

		template <class U>
		struct rebind
		{
			typedef allocator<U> other;
		};
	};

	template <class T>
	class allocator
	{
	public:
		typedef size_t      size_type;
		typedef ptrdiff_t   difference_type;
		typedef T*      pointer;
		typedef const T*    const_pointer;
		typedef T&      reference;
		typedef const T&    const_reference;
		typedef T       value_type;

		template <class U>
		struct rebind
		{
			typedef allocator<U> other;
		};

		allocator() throw()
		{
		}
		template <class U>
		allocator(const allocator<U>& u) throw()
		{
		}
		~allocator() throw()
		{
		}

		pointer address(reference r) const
		{
			return &r;
		}
		const_pointer address(const_reference r) const
		{
			return &r;
		}
		size_type max_size() const throw()
		{
			return 0xFFFFFFFF/sizeof(T);//return std::numeric_limits<size_t>::max()/sizeof(T);
		}
		pointer allocate(size_type n, allocator<void>::const_pointer hint = 0)
		{
			g_bytesAllocated += n * sizeof(T);
			++g_numAllocs;
			return reinterpret_cast<pointer>(::operator new(n * sizeof(T)));
		}
		void deallocate(pointer p, size_type n)
		{
			g_bytesAllocated -= n * sizeof(T);
			--g_numAllocs;
			::operator delete(p);
		}

		void construct(pointer p, const_reference val)
		{
			::new(p) T(val);
		}
		void destroy(pointer p)
		{
			p->~T();
		}
	};

	template <class T1, class T2>
	bool operator==(const allocator<T1>& a1, const allocator<T2>& a2) throw()
	{
		return true;
	}

	template <class T1, class T2>
	bool operator!=(const allocator<T1>& a1, const allocator<T2>& a2) throw()
	{
		return false;
	}

}

//=================================================================================================
//! @example SampleHamt.cpp
//! Informations on the usage the map container of type Hamt
//=================================================================================================
namespace sample
{
	//==================================================================================================
	//! @brief		Test using the Hamt container	
	//==================================================================================================
	void SampleHamt()
	{
		#define kuTestCount (10000)
		#define	kuPoolSize	1024
		CMgr::Log.Log(CLog::keLog_Game, zenConst::kzLineA40);
		CMgr::Log.Log(CLog::keLog_Game, " HASH TABLE (%i entries tested)", kuTestCount);
		CMgr::Log.Log(CLog::keLog_Game, zenConst::kzLineA40);

		zenU32 uValKey(0);
		zenU64 uTimeStartGet,uTimeStartSet;
		zenU64 uTimeStopGet, uTimeStopSet;
		zenU32 uTemp(0);
		

		//---------------------------------------------------------------------------
		// Small hamt test
		//---------------------------------------------------------------------------
		zenMap<zenU32>::Key32 dArraySize(kuPoolSize);
		srand(1);
		uTimeStartSet = zenSys::GetTimeUSec();
		for(zenU32 i=0; i<kuTestCount; ++i)
		{	
			uValKey = rand()<<16 | rand();
			dArraySize.GetAdd(uValKey) = i;
		}
		uTimeStopSet = zenSys::GetTimeUSec();
		srand(1);
		uTimeStartGet = zenSys::GetTimeUSec();
		for(zenU32 i=0; i<kuTestCount; ++i)
		{	
			uValKey = rand()<<16 | rand();
			uTemp += dArraySize[uValKey];
		}
		uTimeStopGet = zenSys::GetTimeUSec();
		CMgr::Log.Log(CLog::keLog_Game, "SparseArray:small32 Set(%7ius) Get(%7ius) Mem(%6iKb) Overhead(%6iKb)", 
			zenU32(uTimeStopSet-uTimeStartSet), 
			zenU32(uTimeStopGet-uTimeStartGet),
			zenU32(dArraySize.GetMemoryFootprint())/1024, 
			zenU32(dArraySize.GetMemoryFootprint()-kuTestCount*sizeof(zenU32))/1024 );

		//---------------------------------------------------------------------------
		// Fast hamt test
		//---------------------------------------------------------------------------
		/*
		CTyp::HamtFast<zenU32, zenU32, zenU16, 4> dArraySpeed(kuPoolSize);
		srand(1);
		dArraySpeed.Set(0xFFFFFFFF, 0);
		dArraySpeed.Set(0xFFFFFFF7, 0);
		uTimeStartSet = zenSys::GetTimeUSec();
		for(zenU32 i=0; i<kuTestCount; ++i)
		{	
			uValKey = rand()<<16 | rand();
			dArraySpeed.[uValKey] = i;
		}
		uTimeStopSet = zenSys::GetTimeUSec();
		srand(1);
		uTimeStartGet = zenSys::GetTimeUSec();
		for(zenU32 i=0; i<kuTestCount; ++i)
		{			
			uValKey = rand()<<16 | rand();
			uTemp += dArraySpeed[uValKey];
		}
		uTimeStopGet = zenSys::GetTimeUSec();
		CMgr::Log.Log(CLog::keLog_Game, "SparseArray:fast32  Set(%7ius) Get(%7ius) Mem(%6iKb) Overhead(%6iKb)", 
			zenU32(uTimeStopSet-uTimeStartSet), 
			zenU32(uTimeStopGet-uTimeStartGet),
			zenU32(dArraySpeed.GetMemoryFootprint())/1024, 
			zenU32(dArraySpeed.GetMemoryFootprint()-kuTestCount*sizeof(zenU32))/1024 );
		*/
		//---------------------------------------------------------------------------
		// std::unordered_map test
		//---------------------------------------------------------------------------
		typedef std::unordered_map<zenU32, zenU32, std::tr1::hash<zenU32>, std::equal_to<zenU32>, tracking_allocator::allocator<zenU32> > hashsettype;

		hashsettype hashset;
		srand(1);
		uTimeStartSet = zenSys::GetTimeUSec();
		for(zenU32 i=0; i<kuTestCount; ++i)
		{			
			uValKey = rand()<<16 | rand();
			hashset[uValKey] = i;
		}
		uTimeStopSet = zenSys::GetTimeUSec();
		srand(1);
		uTimeStartGet = zenSys::GetTimeUSec();
		for(zenU32 i=0; i<kuTestCount; ++i)
		{			
			uValKey = rand()<<16 | rand();
			uTemp += hashset[uValKey];
		}
		uTimeStopGet = zenSys::GetTimeUSec();
		CMgr::Log.Log(CLog::keLog_Game, "std::unordered_map  Set(%7ius) Get(%7ius) Mem(%6iKb) Overhead(%6iKb)", 
			zenU32(uTimeStopSet-uTimeStartSet), 
			zenU32(uTimeStopGet-uTimeStartGet),
			zenU32(tracking_allocator::g_bytesAllocated)/1024, 
			zenU32(tracking_allocator::g_bytesAllocated-kuTestCount*sizeof(zenU32))/1024 );
		
		//---------------------------------------------------------------------------
		// std::map test
		//---------------------------------------------------------------------------
		typedef std::map<zenU32, zenU32, std::less<zenU32>, tracking_allocator::allocator<zenU32> > hashmap; 
		hashmap hashmaptest;
		srand(1);
		size_t AllocSizeStart = tracking_allocator::g_bytesAllocated;
		uTimeStartSet = zenSys::GetTimeUSec();
		for(zenU32 i=0; i<kuTestCount; ++i)
		{			
			uValKey = uValKey<<16 | rand();
			hashmaptest[uValKey] = i;
		}
		uTimeStopSet = zenSys::GetTimeUSec();
		uTimeStartGet = zenSys::GetTimeUSec();
		for(zenU32 i=0; i<kuTestCount; ++i)
		{			
			uValKey = uValKey<<16 | rand();
			uTemp += hashmaptest[uValKey];
		}
		uTimeStopGet = zenSys::GetTimeUSec();
		CMgr::Log.Log(CLog::keLog_Game, "std::map            Set(%7ius) Get(%7ius) Mem(%6iKb) Overhead(%6iKb)", 
			zenU32(uTimeStopSet-uTimeStartSet), 
			zenU32(uTimeStopGet-uTimeStartGet),
			zenU32(tracking_allocator::g_bytesAllocated-AllocSizeStart)/1024, 
			zenU32(tracking_allocator::g_bytesAllocated-AllocSizeStart-kuTestCount*sizeof(zenU32))/1024 );

		//dArraySpeed.Clear();
		CMgr::Log.Log(CLog::keLog_Game, ".... Cleared", uTemp);		
	}

}