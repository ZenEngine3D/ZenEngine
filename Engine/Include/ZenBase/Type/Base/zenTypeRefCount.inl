
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
// zREFERENCE
//=================================================================================================
bool zReference::IsValid()const
{
	return mpReference != NULL;
}

bool zReference::operator==(const zReference& _Cmp)
{
	return mpReference == _Cmp.mpReference;
}

bool zReference::operator!=(const zReference& _Cmp)
{
	return mpReference != _Cmp.mpReference;
}

//=================================================================================================
// zREF OWNER
//=================================================================================================
template<class TRefCountedType>
zEngineRefConst<TRefCountedType>::zEngineRefConst()
: Super(NULL)
{
}

template<class TRefCountedType>
zEngineRefConst<TRefCountedType>::zEngineRefConst(TRefCountedType* _pReference)
: Super(_pReference)
{
}

template<class TRefCountedType>
zEngineRefConst<TRefCountedType>::zEngineRefConst(const zEngineRefConst& _Copy)
: Super(_Copy)
{
}

template<class TRefCountedType>
const zEngineRefConst<TRefCountedType>& zEngineRefConst<TRefCountedType>::operator=(TRefCountedType* _pReference)
{	
	zReference::operator=(_pReference);
	return *this;
}

template<class TRefCountedType>
const zEngineRefConst<TRefCountedType>& zEngineRefConst<TRefCountedType>::operator=(const zEngineRefConst& _Copy)
{	
	zReference::operator=(_Copy);
	return *this;
}

template<class TRefCountedType>
zEngineRef<TRefCountedType>::zEngineRef()
: Super()
{
}

template<class TRefCountedType>
zEngineRef<TRefCountedType>::zEngineRef(TRefCountedType* _pReference)
: Super(_pReference)
{
}

template<class TRefCountedType>
zEngineRef<TRefCountedType>::zEngineRef(const zEngineRef& _Copy)
: Super(_Copy)
{
}

template<class TRefCountedType>
const zEngineRef<TRefCountedType>& zEngineRef<TRefCountedType>::operator=(TRefCountedType* _pReference)
{	
	zReference::operator=(_pReference);
	return *this;
}

template<class TRefCountedType>
const zEngineRef<TRefCountedType>& zEngineRef<TRefCountedType>::operator=(const zEngineRef& _Copy)
{	
	zReference::operator=(_Copy);
	return *this;
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
	TRefCountedType* zEngineRef<TRefCountedType>::Get()
	{
		return static_cast<TRefCountedType*>(mpReference);
	}

	//=================================================================================================
	//! @brief		Return pointer to underlying resource
	//! @details	Useful to let engine access the underlying object
	//! @note		Only use on engine side, else won't compile
	//-------------------------------------------------------------------------------------------------
	//! @return		Object reference
	//=================================================================================================
	template<class TRefCountedType>
	const TRefCountedType* zEngineRefConst<TRefCountedType>::Get()const
	{
		return static_cast<const TRefCountedType*>(mpReference);
	}

	//=================================================================================================
	//! @brief		Return pointer to underlying resource
	//! @details	Useful to let engine access the underlying object
	//! @note		Only use on engine side, else won't compile
	//-------------------------------------------------------------------------------------------------
	//! @return		Object reference
	//=================================================================================================
	template<class TRefCountedType>
	TRefCountedType* zEngineRef<TRefCountedType>::operator->()
	{
		return static_cast<TRefCountedType*>(mpReference);;
	}

	//=================================================================================================
	//! @brief		Return pointer to underlying resource
	//! @details	Useful to let engine access the underlying object
	//! @note		Only use on engine side, else won't compile
	//-------------------------------------------------------------------------------------------------
	//! @return		Object reference
	//=================================================================================================
	template<class TRefCountedType>
	const TRefCountedType* zEngineRefConst<TRefCountedType>::operator->()const
	{
		return static_cast<const TRefCountedType*>(mpReference);;
	}
#endif //ZEN_ENGINELIB


//=================================================================================================
// zSHARED POINTER
//=================================================================================================
template<class TRefCountedType>
zGameRefConst<TRefCountedType>::zGameRefConst()
: Super()
{
}

template<class TRefCountedType>
zGameRefConst<TRefCountedType>::zGameRefConst(TRefCountedType* _pReference)
: Super(_pReference)
{
}

template<class TRefCountedType>
zGameRefConst<TRefCountedType>::zGameRefConst(const zGameRefConst<TRefCountedType>& _Copy)
: Super(_Copy)
{
}

template<class TRefCountedType>
const TRefCountedType* zGameRefConst<TRefCountedType>::Get()const
{
	return static_cast<const TRefCountedType*>(mpReference);
}

template<class TRefCountedType>
const TRefCountedType* zGameRefConst<TRefCountedType>::operator->()const
{
	return Get();
}

template<class TRefCountedType>
const zGameRefConst<TRefCountedType>& zGameRefConst<TRefCountedType>::operator=(TRefCountedType* _pReference)
{	
	zReference::operator=(_pReference);
	return *this;
}

template<class TRefCountedType>
const zGameRefConst<TRefCountedType>& zGameRefConst<TRefCountedType>::operator=(const zGameRefConst& _Copy)
{	
	zReference::operator=(_Copy);
	return *this;
}

template<class TRefCountedType>
zGameRef<TRefCountedType>::zGameRef()
: Super()
{
}

template<class TRefCountedType>
zGameRef<TRefCountedType>::zGameRef(TRefCountedType* _pReference)
: Super(_pReference)
{
}

template<class TRefCountedType>
zGameRef<TRefCountedType>::zGameRef(const zEngineRef<TRefCountedType>& _Copy)
: Super(_Copy)
{
}

template<class TRefCountedType>
const zGameRef<TRefCountedType>& zGameRef<TRefCountedType>::operator=(TRefCountedType* _pReference)
{	
	zReference::operator=(_pReference);
	return *this;
}

template<class TRefCountedType>
const zGameRef<TRefCountedType>& zGameRef<TRefCountedType>::operator=(const zGameRef& _Copy)
{	
	zReference::operator=(_Copy);
	return *this;
}

template<class TRefCountedType>
TRefCountedType* zGameRef<TRefCountedType>::Get()
{
	return static_cast<TRefCountedType*>(mpReference);
}


template<class TRefCountedType>
TRefCountedType* zGameRef<TRefCountedType>::operator->()
{
	return static_cast<TRefCountedType*>(mpReference);
}


}} // namespace zen, zenType

