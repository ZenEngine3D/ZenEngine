
namespace zen { namespace zenType {

template<bool TAutoDelete>
zenRefCounted<TAutoDelete>::zenRefCounted()
: miRefCount(0)
{}

template<bool TAutoDelete>
zenRefCounted<TAutoDelete>::~zenRefCounted()
{}

template<bool TAutoDelete>
void zenRefCounted<TAutoDelete>::ReferenceAdd()
{
	++miRefCount;
}

template<bool TAutoDelete>
void zenRefCounted<TAutoDelete>::ReferenceRem()
{
	--miRefCount; 
	if(miRefCount<=0 && TAutoDelete) 
		delete this; 
}

template<bool TAutoDelete>
int zenRefCounted<TAutoDelete>::ReferenceGet()
{
	return miRefCount;
}


template<class TRefCountedType>
zenSharedPtr<TRefCountedType>::zenSharedPtr()
: mpReference(NULL)
{
}

template<class TRefCountedType>
zenSharedPtr<TRefCountedType>::zenSharedPtr(TRefCountedType* _pReference)
: mpReference(_pReference)
{
	if( mpReference )
		mpReference->ReferenceAdd();
}

template<class TRefCountedType>
zenSharedPtr<TRefCountedType>::~zenSharedPtr()
{
	if( mpReference )
		mpReference->ReferenceRem();
}

template<class TRefCountedType>
void zenSharedPtr<TRefCountedType>::operator=(TRefCountedType* _pReference)
{
	if( mpReference )
		mpReference->ReferenceRem();
	mpReference = _pReference;
	if( mpReference )
		mpReference->ReferenceAdd();
	return *this;
}

template<class TRefCountedType>
bool zenSharedPtr<TRefCountedType>::operator==(const zenSharedPtr& _Cmp)
{
	return mpReference == _Cmp.mpReference;
}

template<class TRefCountedType>
bool zenSharedPtr<TRefCountedType>::operator!=(const zenSharedPtr& _Cmp)
{
	return mpReference != _Cmp.mpReference;
}

template<class TRefCountedType>
bool zenSharedPtr<TRefCountedType>::IsValid()
{
	return mpReference != NULL;
}

template<class TRefCountedType>
TRefCountedType* zenSharedPtr<TRefCountedType>::operator->()
{
	AWAssert(mpReference);
	return mpReference;
}

template<class TRefCountedType>
const TRefCountedType* zenSharedPtr<TRefCountedType>::operator->()const
{
	AWAssert(mpReference);
	return mpReference;
}

}} // namespace zen, zenType

