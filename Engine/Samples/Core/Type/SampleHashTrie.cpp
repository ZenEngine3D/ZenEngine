#include "zenEngine.h"
#include <stdio.h>//test
#include <stdlib.h>
#include <time.h>
#include <map>
#include <set>
#include <unordered_map>
#include <eastl/map.h>

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
		zenIO::Log(zenConst::keLog_Game, zenConst::kzLineA40);
		zenIO::Log(zenConst::keLog_Game, " HASH TABLE (%i entries tested)", kuTestCount);
		zenIO::Log(zenConst::keLog_Game, zenConst::kzLineA40);

		zU32 uValKey(0);
		zU64 uTimeStartGet,uTimeStartSet;
		zU64 uTimeStopGet, uTimeStopSet;
		zU32 uTemp(0);
		

		//---------------------------------------------------------------------------
		// Small hamt test
		//---------------------------------------------------------------------------
		zMap<zU32>::Key32 dArraySize(kuPoolSize);
		srand(1);
		uTimeStartSet = zenSys::GetTimeUSec();
		for(zU32 i=0; i<kuTestCount; ++i)
		{	
			uValKey = rand()<<16 | rand();
			dArraySize.GetAdd(uValKey) = i;
		}
		uTimeStopSet = zenSys::GetTimeUSec();
		srand(1);
		uTimeStartGet = zenSys::GetTimeUSec();
		for(zU32 i=0; i<kuTestCount; ++i)
		{	
			uValKey = rand()<<16 | rand();
			uTemp += dArraySize[uValKey];
		}
		uTimeStopGet = zenSys::GetTimeUSec();
		zenIO::Log(zenConst::keLog_Game, "SparseArray:small32 Set(%7ius) Get(%7ius) Mem(%6iKb) Overhead(%6iKb)", 
			zU32(uTimeStopSet-uTimeStartSet), 
			zU32(uTimeStopGet-uTimeStartGet),
			zU32(dArraySize.GetMemoryFootprint())/1024, 
			zU32(dArraySize.GetMemoryFootprint()-kuTestCount*sizeof(zU32))/1024 );

		//---------------------------------------------------------------------------
		// Fast hamt test
		//---------------------------------------------------------------------------
		/*
		CTyp::HamtFast<zU32, zU32, zU16, 4> dArraySpeed(kuPoolSize);
		srand(1);
		dArraySpeed.Set(0xFFFFFFFF, 0);
		dArraySpeed.Set(0xFFFFFFF7, 0);
		uTimeStartSet = zenSys::GetTimeUSec();
		for(zU32 i=0; i<kuTestCount; ++i)
		{	
			uValKey = rand()<<16 | rand();
			dArraySpeed.[uValKey] = i;
		}
		uTimeStopSet = zenSys::GetTimeUSec();
		srand(1);
		uTimeStartGet = zenSys::GetTimeUSec();
		for(zU32 i=0; i<kuTestCount; ++i)
		{			
			uValKey = rand()<<16 | rand();
			uTemp += dArraySpeed[uValKey];
		}
		uTimeStopGet = zenSys::GetTimeUSec();
		zenIO::Log(zenConst::keLog_Game, "SparseArray:fast32  Set(%7ius) Get(%7ius) Mem(%6iKb) Overhead(%6iKb)", 
			zU32(uTimeStopSet-uTimeStartSet), 
			zU32(uTimeStopGet-uTimeStartGet),
			zU32(dArraySpeed.GetMemoryFootprint())/1024, 
			zU32(dArraySpeed.GetMemoryFootprint()-kuTestCount*sizeof(zU32))/1024 );
		*/
		//---------------------------------------------------------------------------
		// std::unordered_map test
		//---------------------------------------------------------------------------
		typedef std::unordered_map<zU32, zU32, std::hash<zU32>, std::equal_to<zU32>, tracking_allocator::allocator<zU32> > hashsettype;

		hashsettype hashset;
		srand(1);
		uTimeStartSet = zenSys::GetTimeUSec();
		for(zU32 i=0; i<kuTestCount; ++i)
		{			
			uValKey = rand()<<16 | rand();
			hashset[uValKey] = i;
		}
		uTimeStopSet = zenSys::GetTimeUSec();
		srand(1);
		uTimeStartGet = zenSys::GetTimeUSec();
		for(zU32 i=0; i<kuTestCount; ++i)
		{			
			uValKey = rand()<<16 | rand();
			uTemp += hashset[uValKey];
		}
		uTimeStopGet = zenSys::GetTimeUSec();
		zenIO::Log(zenConst::keLog_Game, "std::unordered_map  Set(%7ius) Get(%7ius) Mem(%6iKb) Overhead(%6iKb)", 
			zU32(uTimeStopSet-uTimeStartSet), 
			zU32(uTimeStopGet-uTimeStartGet),
			zU32(tracking_allocator::g_bytesAllocated)/1024, 
			zU32(tracking_allocator::g_bytesAllocated-kuTestCount*sizeof(zU32))/1024 );
		
		//---------------------------------------------------------------------------
		// std::map test
		//---------------------------------------------------------------------------
		{
		typedef std::map<zU32, zU32, std::less<zU32>, tracking_allocator::allocator<zU32> > hashmap; 
		hashmap hashmaptest;
		srand(1);
		size_t AllocSizeStart = tracking_allocator::g_bytesAllocated;
		uTimeStartSet = zenSys::GetTimeUSec();
		for(zU32 i=0; i<kuTestCount; ++i)
		{			
			uValKey = uValKey<<16 | rand();
			hashmaptest[uValKey] = i;
		}
		uTimeStopSet = zenSys::GetTimeUSec();
		uTimeStartGet = zenSys::GetTimeUSec();
		for(zU32 i=0; i<kuTestCount; ++i)
		{			
			uValKey = uValKey<<16 | rand();
			uTemp += hashmaptest[uValKey];
		}
		uTimeStopGet = zenSys::GetTimeUSec();
		zenIO::Log(zenConst::keLog_Game, "std::map            Set(%7ius) Get(%7ius) Mem(%6iKb) Overhead(%6iKb)", 
			zU32(uTimeStopSet-uTimeStartSet), 
			zU32(uTimeStopGet-uTimeStartGet),
			zU32(tracking_allocator::g_bytesAllocated-AllocSizeStart)/1024, 
			zU32(tracking_allocator::g_bytesAllocated-AllocSizeStart-kuTestCount*sizeof(zU32))/1024 );

		//dArraySpeed.Clear();
		zenIO::Log(zenConst::keLog_Game, ".... Cleared", uTemp);		
		}
		//---------------------------------------------------------------------------
		// eastl:map test
		//---------------------------------------------------------------------------		
		{
		typedef eastl::map< zU32, zU32, std::less<zU32>/*, tracking_allocator::allocator<zU32>*/ > eahashmap; 
		eahashmap hashmaptest;
		srand(1);
		size_t AllocSizeStart = tracking_allocator::g_bytesAllocated;
		uTimeStartSet = zenSys::GetTimeUSec();
		for(zU32 i=0; i<kuTestCount; ++i)
		{			
			uValKey = uValKey<<16 | rand();
			hashmaptest[uValKey] = i;
		}
		uTimeStopSet = zenSys::GetTimeUSec();
		uTimeStartGet = zenSys::GetTimeUSec();
		for(zU32 i=0; i<kuTestCount; ++i)
		{			
			uValKey = uValKey<<16 | rand();
			uTemp += hashmaptest[uValKey];
		}
		uTimeStopGet = zenSys::GetTimeUSec();
		zenIO::Log(zenConst::keLog_Game, "eastl::map          Set(%7ius) Get(%7ius) Mem(%6iKb) Overhead(%6iKb)", 
			zU32(uTimeStopSet-uTimeStartSet), 
			zU32(uTimeStopGet-uTimeStartGet),
			zU32(tracking_allocator::g_bytesAllocated-AllocSizeStart)/1024, 
			zU32(tracking_allocator::g_bytesAllocated-AllocSizeStart-kuTestCount*sizeof(zU32))/1024 );

		//dArraySpeed.Clear();
		zenIO::Log(zenConst::keLog_Game, ".... Cleared", uTemp);		
		}
		//---------------------------------------------------------------------------
		// Find First Unused
		//---------------------------------------------------------------------------
//! @todo 0 assert here
/*
		zMap<char>::Key32 hashIndex(32);
		hashIndex.Set(0, 'a');
		hashIndex.Set(1, 'b');
		hashIndex.Set(2, 'c');
		hashIndex.Set(4, 'e');
		hashIndex.Set(5, 'f');
		zHash32 firstUnused = hashIndex.GetFirstUnusedKey();
		zenIO::Log(zenConst::keLog_Game, "Hashmap Find first Unused value OK? : %s", zenConst::kzFalseTrue[(zUInt)firstUnused==3] );
*/
	}

}