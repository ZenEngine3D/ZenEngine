#pragma once


namespace zen{ namespace zenMem
{
	enum eAllocFlags
	{
		keFlag_WantPool,
		keFlag_WantOutOfBoundCheck,
		keFlag__Count
	};

	using AllocFlags = zFlag<eAllocFlags, eAllocFlags::keFlag__Count, zU8>;
}}

void* operator new(size_t _Size, const char* _Filename, int _LineNumber, size_t _MaxSize, bool _IsPoolItem, bool _IsCheckAccess);
void* operator new[](size_t _Size, const char* _Filename, int _LineNumber, size_t _MaxSize, bool _IsPoolItem, bool _IsCheckAccess);

#define zenVariableSize						1

#define	zenDel(_Pointer_)					{ delete	_Pointer_; }
#define	zenDelArray(_Pointer_)				{ delete[]	_Pointer_; }
#define	zenDelNullptr(_Pointer_)			{ delete	_Pointer_; _Pointer_=nullptr; }
#define	zenDelArrayNullptr(_Pointer_)		{ delete[]	_Pointer_; _Pointer_=nullptr; }

#define	zenNew								new(__FILE__, __LINE__, 0, false, false)
#define	zenNewWithResize(_SizeMax_)			new(__FILE__, __LINE__, _SizeMax_, false, false)
#define zenNewPool							new(__FILE__, __LINE__, 0, true, false)
#define zenNewCheck							new(__FILE__, __LINE__, 0, true, true)
#define	zenNewWithResizeCheck(_SizeMax_)	new(__FILE__, __LINE__, _SizeMax_, false, false)
#define zenNewPoolCheck						new(__FILE__, __LINE__, 0, true, true)

namespace zen{ namespace zenMem
{
	size_t GetRequestedSize(void* _pMemory);
	void* ResizeMemory(void* _pMemory, size_t _NewSize);	

	template<class TType>
	TType* Resize(TType* _pObject, size_t _NewObjectCount)
	{	
		size_t OldCount	= GetRequestedSize(_pObject) / sizeof(TType);		
		while(OldCount > _NewObjectCount)
			_pObject[--OldCount].~TType();
		
		_pObject = reinterpret_cast<TType*>(ResizeMemory(_pObject, sizeof(TType)*_NewObjectCount ));
		
		while(OldCount < _NewObjectCount)
			new(&_pObject[OldCount++]) TType();
		
		return _pObject;
	}
} }