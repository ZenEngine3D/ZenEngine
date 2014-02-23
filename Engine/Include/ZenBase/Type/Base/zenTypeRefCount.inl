
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

}} // namespace zen, zenType

