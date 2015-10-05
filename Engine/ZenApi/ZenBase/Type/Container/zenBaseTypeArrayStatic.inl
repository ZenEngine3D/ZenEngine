namespace zen { namespace zenType {

template<class TType>
zArrayStatic<TType>::zArrayStatic()
: zArrayBase()
{
}

template<class TType>
zArrayStatic<TType>::zArrayStatic(zUInt _uCount)
: zArrayBase()
{
	muCount	= _uCount;
	mpData	= zenNewDefault TType[muCount];			
}

template<class TType>
zArrayStatic<TType>::zArrayStatic(const TType* _pCopy, zUInt _uCount, zUInt _uExtraCount=0)
: zArrayBase()
{		
	muCount			= _uCount+_uExtraCount;
	mpData			= zenNewDefault TType[muCount];			
	if( std::is_trivially_copyable<TType>::value )	
	{
		memcpy(mpData, _pCopy, sizeof(TType)*_uCount);
	}
	else				
	{				
		TType* pItemCur	= mpData;
		TType* pItemEnd	= mpData+_uCount;
		while( pItemCur < pItemEnd )
			*pItemCur++ = *_pCopy++;
	}
}	

template<class TType>
zArrayStatic<TType>::zArrayStatic(const zArrayStatic& _Copy, zUInt _uExtraCount=0)
{					
	muCount					= _Copy.Count()+_uExtraCount;
	mpData					= zenNewDefault TType[muCount];		
	const TType* pItemSrc	= _Copy.First();
	if( std::is_trivially_copyable<TType>::value )
	{
		memcpy(mpData, pItemSrc, sizeof(TType)*muCount);
	}
	else				
	{	
		TType* pItemCur	= mpData;
		TType* pItemEnd	= mpData+muCount;
		while( pItemCur < pItemEnd )
			*pItemCur++ = *pItemSrc++;
	}
}
		
template<class TType>
zArrayStatic<TType>::~zArrayStatic()
{
	zenDelNullArray(mpData);
}
		
template<class TType>
zUInt zArrayStatic<TType>::SetCount(zUInt _uCount)
{
	zenDelNullArray(mpData);			
	if( _uCount )	mpData = zenNewDefault TType[_uCount];			
	else			mpData = NULL;
	muCount	= _uCount;
	return muCount;
}

template<class TType>	
zUInt zArrayStatic<TType>::SetCountNoConstructor(zUInt _uCount)
{
	zenDelNullArray(mpData);			
	if( _uCount )	mpData	= (TType*) zenNewDefault zU8[sizeof(TType)*_uCount];
	else			mpData	= NULL;
	muCount	= _uCount;
	return muCount;
}

//==================================================================================================
//! @brief		Exchange the content of 2 arrays
//! @details	Allows to grab the content of 1 array object without needing a
//				memory allocation/copy
//--------------------------------------------------------------------------------------------------
//! @param aCopy - Array to exchange content with		
//==================================================================================================
template<class TType>
void zArrayStatic<TType>::Swap(zArrayStatic<TType>& _Copy)
{
	TType*	pCopyData(_Copy.mpData);
	zUInt	uCopyCount(_Copy.muCount);
	_Copy.mpData	= mpData;
	_Copy.muCount	= muCount;
	mpData			= pCopyData;
	muCount			= uCopyCount;
}


	
} } //namespace zen, Type
