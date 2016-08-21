
namespace zen { namespace zenType {

//=================================================================================================
// zREF COUNTED
//=================================================================================================
void zRefCounted::ReferenceAdd()
{
	++miRefCount;
}

void zRefCounted::ReferenceRem()
{
	//! @todo multihread not safe to have multiple add to list
	if( miRefCount.fetch_sub(1) == 1 && !mLstPendingDelLink.IsInList() )
		sLstPendingDel[suLstPendingDelIndex].PushHead( *this );
}

zInt zRefCounted::ReferenceCount()
{
	return miRefCount;
}

//=================================================================================================
// CLASS: zReference
//=================================================================================================
bool zReference::IsValid()const
{
	return mpReference != nullptr;
}

bool zReference::operator==(const zReference& _Cmp)const
{
	return mpReference == _Cmp.mpReference;
}

bool zReference::operator!=(const zReference& _Cmp)const
{
	return mpReference != _Cmp.mpReference;
}

bool zReference::operator==(const zRefCounted* _pCmp)const
{	
	return mpReference == _pCmp;
}

bool zReference::operator!=(const zRefCounted* _pCmp)const
{
	return mpReference != _pCmp;
}

//=================================================================================================
// CLASS: zEngineRef
//=================================================================================================
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
	Super::operator=(_pReference);
	return *this;
}

template<class TRefCountedType>
const zEngineRef<TRefCountedType>& zEngineRef<TRefCountedType>::operator=(const zEngineRef& _Copy)
{	
	Super::operator=(_Copy);
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
	TRefCountedType* zEngineRef<TRefCountedType>::Get()const
	{
		return static_cast<TRefCountedType*>(mpReference);
	}
	
	template<class TRefCountedType>
	TRefCountedType* zEngineRef<TRefCountedType>::GetSafe()const
	{
		zenAssert(IsValid());
		return static_cast<TRefCountedType*>(mpReference);
	}

	template<class TRefCountedType>
	TRefCountedType* zEngineRef<TRefCountedType>::operator->()const
	{
		return static_cast<TRefCountedType*>(mpReference);
	}
#endif //ZEN_ENGINELIB

//=================================================================================================
// CLASS: zEngineConstRef
//=================================================================================================
template<class TRefCountedType>
zEngineConstRef<TRefCountedType>::zEngineConstRef()
: Super(nullptr)
{
}

template<class TRefCountedType>
zEngineConstRef<TRefCountedType>::zEngineConstRef(const TRefCountedType* _pReference)
: Super(const_cast<TRefCountedType*>(_pReference))
{
}

template<class TRefCountedType>
zEngineConstRef<TRefCountedType>::zEngineConstRef(const zEngineConstRef& _Copy)
: Super(_Copy)
{
}

template<class TRefCountedType>
zEngineConstRef<TRefCountedType>::zEngineConstRef(const zEngineRef<TRefCountedType>& _Copy)
: Super(_Copy)
{
}

template<class TRefCountedType>
const zEngineConstRef<TRefCountedType>& zEngineConstRef<TRefCountedType>::operator=(const TRefCountedType* _pReference)
{	
	Super::operator=(_pReference);
	return *this;
}

template<class TRefCountedType>
const zEngineConstRef<TRefCountedType>& zEngineConstRef<TRefCountedType>::operator=(const zEngineConstRef& _Copy)
{	
	Super::operator=(_Copy);
	return *this;
}

template<class TRefCountedType>
const zEngineConstRef<TRefCountedType>& zEngineConstRef<TRefCountedType>::operator=(const zEngineRef<TRefCountedType>& _Copy)
{	
	Super::operator=(_Copy);
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
	const TRefCountedType* zEngineConstRef<TRefCountedType>::Get()const
	{
		return static_cast<const TRefCountedType*>(mpReference);
	}

	template<class TRefCountedType>
	const TRefCountedType* zEngineConstRef<TRefCountedType>::GetSafe()const
	{
		zenAssert(IsValid());
		return static_cast<const TRefCountedType*>(mpReference);
	}

	template<class TRefCountedType>
	const TRefCountedType* zEngineConstRef<TRefCountedType>::operator->()const
	{
		return static_cast<const TRefCountedType*>(mpReference);
	}

#endif //ZEN_ENGINELIB

//=================================================================================================
// CLASS: zGameRef
//=================================================================================================
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
	Super::operator=(_pReference);
	return *this;
}

template<class TRefCountedType>
const zGameRef<TRefCountedType>& zGameRef<TRefCountedType>::operator=(const zGameRef& _Copy)
{	
	Super::operator=(_Copy);
	return *this;
}

template<class TRefCountedType>
TRefCountedType* zGameRef<TRefCountedType>::Get()const
{
	return static_cast<TRefCountedType*>(mpReference);
}

template<class TRefCountedType>
TRefCountedType* zGameRef<TRefCountedType>::GetSafe()const
{
	zenAssert(IsValid());
	return static_cast<TRefCountedType*>(mpReference);
}

template<class TRefCountedType>
TRefCountedType* zGameRef<TRefCountedType>::operator->()const
{
	return static_cast<TRefCountedType*>(mpReference);
}

//=================================================================================================
// CLASS: zGameConstRef
//=================================================================================================
template<class TRefCountedType>
zGameConstRef<TRefCountedType>::zGameConstRef()
: Super()
{
}

template<class TRefCountedType>
zGameConstRef<TRefCountedType>::zGameConstRef(const TRefCountedType* _pReference)
: Super( const_cast<TRefCountedType*>(_pReference) )
{
}

template<class TRefCountedType>
zGameConstRef<TRefCountedType>::zGameConstRef(const zGameConstRef& _Copy)
: Super(_Copy)
{
}

template<class TRefCountedType>
zGameConstRef<TRefCountedType>::zGameConstRef(const zGameRef<TRefCountedType>& _Copy)
: Super(_Copy)
{
}

template<class TRefCountedType>
const zGameConstRef<TRefCountedType>& zGameConstRef<TRefCountedType>::operator=(const TRefCountedType* _pReference)
{	
	Super::operator=(const_cast<TRefCountedType*>(_pReference));
	return *this;
}

template<class TRefCountedType>
const zGameConstRef<TRefCountedType>& zGameConstRef<TRefCountedType>::operator=(const zGameConstRef& _Copy)
{	
	Super::operator=(_Copy);
	return *this;
}

template<class TRefCountedType>
const zGameConstRef<TRefCountedType>& zGameConstRef<TRefCountedType>::operator=(const zGameRef<TRefCountedType>& _Copy)
{	
	Super::operator=(_Copy);
	return *this;
}

template<class TRefCountedType>
const TRefCountedType* zGameConstRef<TRefCountedType>::Get()const
{
	return static_cast<const TRefCountedType*>(mpReference);
}

template<class TRefCountedType>
const TRefCountedType* zGameConstRef<TRefCountedType>::GetSafe()const
{
	zenAssert(IsValid());
	return static_cast<const TRefCountedType*>(mpReference);
}

template<class TRefCountedType>
const TRefCountedType* zGameConstRef<TRefCountedType>::operator->()const
{
	return Get();
}


}} // namespace zen, zenType

