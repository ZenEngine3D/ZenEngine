
namespace zen { namespace zenMem
{

template<class TType>
void Copy(TType* _pDestination, const TType* _pSource, zUInt _uCount)
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

}} // namespace zen { namespace zenMem

