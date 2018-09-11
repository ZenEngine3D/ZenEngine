#pragma once

namespace zbMem
{
	class Allocator_Base
	{
	zenClassDeclareNoParent(Allocator_Base);
	public:
						Allocator_Base()=default;	
	protected:		
		bool			ShouldAccessCheck(size_t _Size, size_t _SizeMax, const char* _Filename, int _LineNumber, bool _IsArrayNew, bool _IsPoolItem, bool _IsCheckAccess)const;
		DebugTracking	mDebugTracking;
	};
}  

#include zenHeaderPlatform(zbMemAllocator)

namespace zbMem
{
	class Allocator : public Allocator_HAL
	{
	zenClassDeclare(Allocator, Allocator_HAL);	
	public:	
						Allocator()=default;
		void*			Malloc(size_t _Size, size_t _SizeMax, const char* _Filename, int _LineNumber, bool _IsArrayNew, bool _IsPoolItem, bool _IsCheckAccess);
		void* 			Resize(void* _pMemory, size_t _NewSize);		
		void			Free(void* _pMemory, bool _IsArrayDel);
		size_t			GetRequestedSize(void* _pMemory)const;	
	};
}
