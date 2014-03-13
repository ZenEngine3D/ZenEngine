
namespace zen { namespace zenType {

template<bool TAutoDelete>
zRefCounted<TAutoDelete>::zRefCounted()
: miRefCount(0)
{}

template<bool TAutoDelete>
zRefCounted<TAutoDelete>::~zRefCounted()
{}

template<bool TAutoDelete>
void zRefCounted<TAutoDelete>::ReferenceAdd()
{
	++miRefCount;
}

template<bool TAutoDelete>
void zRefCounted<TAutoDelete>::ReferenceRem()
{
	--miRefCount; 
	if(miRefCount<=0 && TAutoDelete) 
		delete this; 
}

template<bool TAutoDelete>
int zRefCounted<TAutoDelete>::ReferenceGet()
{
	return miRefCount;
}

/*
template<class TRefCountedType>
zSharedPtr<TRefCountedType>::zSharedPtr()
: mpReference(NULL)
{
}

template<class TRefCountedType>
zSharedPtr<TRefCountedType>::zSharedPtr(TRefCountedType* _pReference)
: mpReference(_pReference)
{
	if( mpReference )
		mpReference->ReferenceAdd();
}

template<class TRefCountedType>
zSharedPtr<TRefCountedType>::~zSharedPtr()
{
	if( mpReference )
		mpReference->ReferenceRem();
}

template<class TRefCountedType>
const zSharedPtr<TRefCountedType>& zSharedPtr<TRefCountedType>::operator=(TRefCountedType* _pReference)
{
	if( mpReference )
		mpReference->ReferenceRem();
	mpReference = _pReference;
	if( mpReference )
		mpReference->ReferenceAdd();
	return *this;
}

template<class TRefCountedType>
bool zSharedPtr<TRefCountedType>::operator==(const zSharedPtr& _Cmp)
{
	return mpReference == _Cmp.mpReference;
}

template<class TRefCountedType>
bool zSharedPtr<TRefCountedType>::operator!=(const zSharedPtr& _Cmp)
{
	return mpReference != _Cmp.mpReference;
}

template<class TRefCountedType>
bool zSharedPtr<TRefCountedType>::IsValid()
{
	return mpReference != NULL;
}
*/

template<class TRefCountedType>
zRefOwner<TRefCountedType>::zRefOwner()
: mpReference(NULL)
{
}

template<class TRefCountedType>
zRefOwner<TRefCountedType>::zRefOwner(TRefCountedType* _pReference)
: mpReference(_pReference)
{
	if( mpReference )
		mpReference->ReferenceAdd();
}

template<class TRefCountedType>
zRefOwner<TRefCountedType>::zRefOwner(const zRefOwner& _Copy)
: mpReference(_Copy.mpReference)
{
	if( mpReference )
		mpReference->ReferenceAdd();
}

template<class TRefCountedType>
zRefOwner<TRefCountedType>::~zRefOwner()
{
	if( mpReference )
		mpReference->ReferenceRem();
}

template<class TRefCountedType>
const zRefOwner<TRefCountedType>& zRefOwner<TRefCountedType>::operator=(TRefCountedType* _pReference)
{
	if( mpReference )
		mpReference->ReferenceRem();
	mpReference = _pReference;
	if( mpReference )
		mpReference->ReferenceAdd();
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
bool zRefOwner<TRefCountedType>::IsValid()
{
	return mpReference != NULL;
}

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

