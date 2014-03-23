
namespace zen { namespace zenType {

//=================================================================================================
// zREF COUNTED
//=================================================================================================
zRefCounted::zRefCounted()
: miRefCount(0)
{
}

void zRefCounted::ReferenceAdd()
{
	++miRefCount;
}

void zRefCounted::ReferenceRem()
{
	ZENAssert(miRefCount > 0)
	--miRefCount; 
	if(miRefCount<=0) 
		ReferenceNoneCB();
}

zInt zRefCounted::ReferenceCount()
{
	return miRefCount;
}

//=================================================================================================
// zREF OWNER
//=================================================================================================
template<class TRefCountedType>
zRefOwner<TRefCountedType>::zRefOwner()
: mpReference(NULL)
{
}

template<class TRefCountedType>
zRefOwner<TRefCountedType>::zRefOwner(TRefCountedType* _pReference)
: mpReference(NULL)
{
	*this = _pReference;	
}

template<class TRefCountedType>
zRefOwner<TRefCountedType>::zRefOwner(const zRefOwner& _Copy)
: mpReference(NULL)
{
	*this = _Copy.mpReference;
}

template<class TRefCountedType>
zRefOwner<TRefCountedType>::~zRefOwner()
{
	*this = NULL;
}

template<class TRefCountedType>
const zRefOwner<TRefCountedType>& zRefOwner<TRefCountedType>::operator=(TRefCountedType* _pReference)
{	
	if( mpReference )
		reinterpret_cast<zRefCounted*>(mpReference)->ReferenceRem();
	mpReference = _pReference;
	if( mpReference )
		reinterpret_cast<zRefCounted*>(mpReference)->ReferenceAdd();
	
	return *this;
}

template<class TRefCountedType>
bool zRefOwner<TRefCountedType>::operator==(const zRefOwner& _Cmp)
{
	return mpReference == _Cmp.mpReference;
}

template<class TRefCountedType>
bool zRefOwner<TRefCountedType>::operator!=(const zRefOwner& _Cmp)
{
	return mpReference != _Cmp.mpReference;
}

template<class TRefCountedType>
bool zRefOwner<TRefCountedType>::IsValid()const
{
	return mpReference != NULL;
}

#if ZEN_ENGINELIB
	//=================================================================================================
	//! @brief		Return pointer to underlying resource
	//! @details	Useful to let engine access the underlying object
	//! @note		Only use on engine side, else won't compile
	//-------------------------------------------------------------------------------------------------
	//! @return		Object reference
	//=================================================================================================
	template<class TRefCountedType>
	TRefCountedType* zRefOwner<TRefCountedType>::Get()
	{
		return mpReference;
	}

	//=================================================================================================
	//! @brief		Return pointer to underlying resource
	//! @details	Useful to let engine access the underlying object
	//! @note		Only use on engine side, else won't compile
	//-------------------------------------------------------------------------------------------------
	//! @return		Object reference
	//=================================================================================================
	template<class TRefCountedType>
	const TRefCountedType* zRefOwner<TRefCountedType>::Get()const
	{
		return mpReference;
	}

	//=================================================================================================
	//! @brief		Return pointer to underlying resource
	//! @details	Useful to let engine access the underlying object
	//! @note		Only use on engine side, else won't compile
	//-------------------------------------------------------------------------------------------------
	//! @return		Object reference
	//=================================================================================================
	template<class TRefCountedType>
	TRefCountedType* zRefOwner<TRefCountedType>::operator->()
	{
		return mpReference;
	}

	//=================================================================================================
	//! @brief		Return pointer to underlying resource
	//! @details	Useful to let engine access the underlying object
	//! @note		Only use on engine side, else won't compile
	//-------------------------------------------------------------------------------------------------
	//! @return		Object reference
	//=================================================================================================
	template<class TRefCountedType>
	const TRefCountedType* zRefOwner<TRefCountedType>::operator->()const
	{
		return mpReference;
	}
#endif //ZEN_ENGINELIB

//=================================================================================================
// zSHARED POINTER
//=================================================================================================
template<class TRefCountedType>
zSharedPtr<TRefCountedType>::zSharedPtr()
: Super()
{
}

template<class TRefCountedType>
zSharedPtr<TRefCountedType>::zSharedPtr(TRefCountedType* _pReference)
: Super(_pReference)
{
	if( mpReference )
		mpReference->ReferenceAdd();
}

template<class TRefCountedType>
zSharedPtr<TRefCountedType>::zSharedPtr(const zRefOwner<TRefCountedType>& _Copy)
: Super(_Copy)
{
}

template<class TRefCountedType>
TRefCountedType* zSharedPtr<TRefCountedType>::Get()
{
	ZENAssert(mpReference);
	return mpReference;
}

template<class TRefCountedType>
const TRefCountedType* zSharedPtr<TRefCountedType>::Get()const
{
	ZENAssert(mpReference);
	return mpReference;
}

template<class TRefCountedType>
TRefCountedType* zSharedPtr<TRefCountedType>::operator->()
{
	return Get();
}

template<class TRefCountedType>
const TRefCountedType* zSharedPtr<TRefCountedType>::operator->()const
{
	return Get();
}

}} // namespace zen, zenType

