namespace zen { namespace awtype {

template<class TType>
awArrayStatic<TType>::awArrayStatic(CMem::Allocator* _pAllocator=NULL)
: awArrayBase(_pAllocator)
{
}

template<class TType>
awArrayStatic<TType>::awArrayStatic(awUInt _uCount, CMem::Allocator* _pAllocator=NULL)
: awArrayBase(_pAllocator)
{
	muCount	= _uCount;
	mpData	= AWNew(mpAllocator) TType[muCount];			
}

template<class TType>
awArrayStatic<TType>::awArrayStatic(const TType* _pCopy, awUInt _uCount, awUInt _uExtraCount=0, CMem::Allocator* _pAllocator=NULL)
: awArrayBase(_pAllocator)
{		
	muCount			= _uCount+_uExtraCount;
	mpData			= AWNew(mpAllocator) TType[muCount];			
	if( AWSupportsMemCopy<TType>::value )	
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
awArrayStatic<TType>::awArrayStatic(const awArrayStatic& _Copy, awUInt _uExtraCount=0)
{					
	muCount					= _Copy.Count()+_uExtraCount;
	mpAllocator				= _Copy.mpAllocator;
	mpData					= AWNew(mpAllocator) TType[muCount];		
	const TType* pItemSrc	= _Copy.First();
	if( AWSupportsMemCopy<TType>::value )
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
awArrayStatic<TType>::~awArrayStatic()
{
	AWDelNullArray(mpData);
}
		
template<class TType>
awUInt awArrayStatic<TType>::SetCount(awUInt _uCount)
{
	AWDelNullArray(mpData);			
	if( _uCount )	mpData = AWNew(mpAllocator) TType[_uCount];			
	else			mpData = NULL;
	muCount	= _uCount;
	return muCount;
}

template<class TType>	
awUInt awArrayStatic<TType>::SetCountNoConstructor(awUInt _uCount)
{
	AWDelNullArray(mpData);			
	if( _uCount )	mpData	= (TType*) AWNew(mpAllocator) awU8[sizeof(TType)*_uCount];
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
void awArrayStatic<TType>::Swap(awArrayStatic<TType>& _Copy)
{
	TType*	pCopyData(_Copy.mpData);
	awUInt	uCopyCount(_Copy.muCount);
	_Copy.mpData	= mpData;
	_Copy.muCount	= muCount;
	mpData			= pCopyData;
	muCount			= uCopyCount;
}


	
} } //namespace zen, Type
