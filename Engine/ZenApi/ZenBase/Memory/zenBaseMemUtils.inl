
namespace zen { namespace zenMem
{

template<class TType>
void Copy(TType* zenRestrict _pDestination, const TType* zenRestrict _pSource, zUInt _uCount)
{
	if( _pDestination==nullptr || _pSource==nullptr )
		return;

	if( std::is_trivially_copyable<TType>::value )
	{
		zenMem::CopyRaw(_pDestination, _pSource, sizeof(TType)*_uCount);
	}
	else				
	{				
		TType* pItemCur	= _pDestination;
		TType* pItemEnd	= _pDestination+_uCount;
		while( pItemCur < pItemEnd )
			*pItemCur++ = *_pSource++;
	}		
}

template<class TType>
void Zero(TType& _Destination)
{
	Zero( (void*)&_Destination, sizeof(_Destination) );
}

}} // namespace zen { namespace zenMem

