namespace zen { namespace zenType {

template<class TType>
zenArrayStatic<TType>::zenArrayStatic()
: zenArrayBase()
{
}

template<class TType>
zenArrayStatic<TType>::zenArrayStatic(zenUInt _uCount)
: zenArrayBase()
{
	muCount	= _uCount;
	mpData	= zenNewDefault TType[muCount];			
}

template<class TType>
zenArrayStatic<TType>::zenArrayStatic(const TType* _pCopy, zenUInt _uCount, zenUInt _uExtraCount=0)
: zenArrayBase()
{		
	muCount			= _uCount+_uExtraCount;
	mpData			= zenNewDefault TType[muCount];			
	if( ZenSupportsMemCopy<TType>::value )	
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
zenArrayStatic<TType>::zenArrayStatic(const zenArrayStatic& _Copy, zenUInt _uExtraCount=0)
{					
	muCount					= _Copy.Count()+_uExtraCount;
	mpData					= zenNewDefault TType[muCount];		
	const TType* pItemSrc	= _Copy.First();
	if( ZenSupportsMemCopy<TType>::value )
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
zenArrayStatic<TType>::~zenArrayStatic()
{
	zenDelNullArray(mpData);
}
		
template<class TType>
zenUInt zenArrayStatic<TType>::SetCount(zenUInt _uCount)
{
	zenDelNullArray(mpData);			
	if( _uCount )	mpData = zenNewDefault TType[_uCount];			
	else			mpData = NULL;
	muCount	= _uCount;
	return muCount;
}

template<class TType>	
zenUInt zenArrayStatic<TType>::SetCountNoConstructor(zenUInt _uCount)
{
	zenDelNullArray(mpData);			
	if( _uCount )	mpData	= (TType*) zenNewDefault zenU8[sizeof(TType)*_uCount];
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
void zenArrayStatic<TType>::Swap(zenArrayStatic<TType>& _Copy)
{
	TType*	pCopyData(_Copy.mpData);
	zenUInt	uCopyCount(_Copy.muCount);
	_Copy.mpData	= mpData;
	_Copy.muCount	= muCount;
	mpData			= pCopyData;
	muCount			= uCopyCount;
}


	
} } //namespace zen, Type
