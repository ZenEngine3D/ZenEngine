
namespace zen { namespace awtype {

template<bool TAutoDelete>
awRefCounted<TAutoDelete>::awRefCounted()
: miRefCount(0)
{}

template<bool TAutoDelete>
awRefCounted<TAutoDelete>::~awRefCounted()
{}

template<bool TAutoDelete>
void awRefCounted<TAutoDelete>::ReferenceAdd()
{
	++miRefCount;
}

template<bool TAutoDelete>
void awRefCounted<TAutoDelete>::ReferenceRem()
{
	--miRefCount; 
	if(miRefCount<=0 && TAutoDelete) 
		delete this; 
}

template<bool TAutoDelete>
int awRefCounted<TAutoDelete>::ReferenceGet()
{
	return miRefCount;
}

}} // namespace zen, awtype

