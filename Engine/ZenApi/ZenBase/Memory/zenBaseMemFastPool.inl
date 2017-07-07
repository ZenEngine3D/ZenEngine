#pragma once

namespace zen { namespace zenMem
{

size_t zAllocatorFastPoolBase::GetItemSize()const
{
	return muItemSize;
}

size_t zAllocatorFastPoolBase::GetReservedSize()const
{
	return muReservedCount*muItemSize;
}

zU32 zAllocatorFastPoolBase::GetReservedCount()const
{
	return muReservedCount;
}

zU32 zAllocatorFastPoolBase::GetIncreaseCount()const
{
	return muItemIncrease;
}

zU32 zAllocatorFastPoolBase::GetAllocatedCount()const
{
	return muAllocatedCount;
}

void zAllocatorFastPoolBase::SetIncreaseCount(zU32 _uIncreaseCount)
{
	muItemIncrease = _uIncreaseCount;
}

template<class TObject, bool TCallConstructor, bool TCallDestructor>
zAllocatorFastPool<TObject, TCallConstructor, TCallDestructor>::zAllocatorFastPool()
: Super()
{
}

template<class TObject, bool TCallConstructor, bool TCallDestructor>
zAllocatorFastPool<TObject, TCallConstructor, TCallDestructor>::zAllocatorFastPool(zU32 _uItemCount, zU32 _uItemIncrease)
: Super(sizeof(TObject), _uItemCount, _uItemIncrease)
{
}

template<class TObject, bool TCallConstructor, bool TCallDestructor>
void zAllocatorFastPool<TObject, TCallConstructor, TCallDestructor>::Init(zU32 _uItemCount, zU32 _uItemIncrease)
{
	Super::Init(sizeof(TObject), _uItemCount, _uItemIncrease)
}

template<class TObject, bool TCallConstructor, bool TCallDestructor>
TObject* zAllocatorFastPool<TObject, TCallConstructor, TCallDestructor>::Allocate()
{
	void* pMemAlloc = Super::Allocate();
	if( TCallConstructor )	return new(pMemAlloc) TObject();
	else					return reinterpret_cast<TObject*>(pMemAlloc);
}

template<class TObject, bool TCallConstructor, bool TCallDestructor>
void zAllocatorFastPool<TObject, TCallConstructor, TCallDestructor>::Deallocate(TObject* _pAlloc)
{
	if( _pAlloc )
	{
		if( TCallDestructor )
			(*_pAlloc).~TObject();
		Super::Deallocate( _pAlloc );
	}
}

template<class TObject, bool TCallConstructor, bool TCallDestructor> template<class TParam1>
TObject* zAllocatorFastPool<TObject, TCallConstructor, TCallDestructor>::Allocate(TParam1 _Val1)
{
	zenStaticAssertMsg( TCallConstructor, "Constructor not support on this pool");
	void* pMemAlloc = Super::Allocate();
	return new(pMemAlloc) TObject(_Val1);
}

template<class TObject, bool TCallConstructor, bool TCallDestructor> template<class TParam1, class TParam2>
TObject* zAllocatorFastPool<TObject, TCallConstructor, TCallDestructor>::Allocate(TParam1 _Val1, TParam2 _Val2)
{
	zenStaticAssertMsg( TCallConstructor, "Constructor not support on this pool");
	void* pMemAlloc = Super::Allocate();
	return new(pMemAlloc) TObject(_Val1, _Val2);
}

template<class TObject, bool TCallConstructor, bool TCallDestructor> template<class TParam1, class TParam2, class TParam3>
TObject* zAllocatorFastPool<TObject, TCallConstructor, TCallDestructor>::Allocate(TParam1 _Val1, TParam2 _Val2, TParam3 _Val3)
{
	zenStaticAssertMsg( TCallConstructor, "Constructor not support on this pool");
	void* pMemAlloc = Super::Allocate();
	return new(pMemAlloc) TObject(_Val1, _Val2, _Val3);
}


} } //namespace zen { namespace zenMem
