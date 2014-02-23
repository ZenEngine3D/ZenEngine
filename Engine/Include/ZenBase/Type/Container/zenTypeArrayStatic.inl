namespace zen { namespace zenType {

template<class TType>
zenArrayStatic<TType>::zenArrayStatic(CMem::Allocator* _pAllocator=NULL)
: zenArrayBase(_pAllocator)
{
}

template<class TType>
zenArrayStatic<TType>::zenArrayStatic(zenUInt _uCount, CMem::Allocator* _pAllocator=NULL)
: zenArrayBase(_pAllocator)
{
	muCount	= _uCount;
	mpData	= AWNew(mpAllocator) TType[muCount];			
}

template<class TType>
zenArrayStatic<TType>::zenArrayStatic(const TType* _pCopy, zenUInt _uCount, zenUInt _uExtraCount=0, CMem::Allocator* _pAllocator=NULL)
: zenArrayBase(_pAllocator)
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
zenArrayStatic<TType>::zenArrayStatic(const zenArrayStatic& _Copy, zenUInt _uExtraCount=0)
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
zenArrayStatic<TType>::~zenArrayStatic()
{
	AWDelNullArray(mpData);
}
		
template<class TType>
zenUInt zenArrayStatic<TType>::SetCount(zenUInt _uCount)
{
	AWDelNullArray(mpData);			
	if( _uCount )	mpData = AWNew(mpAllocator) TType[_uCount];			
	else			mpData = NULL;
	muCount	= _uCount;
	return muCount;
}

template<class TType>	
zenUInt zenArrayStatic<TType>::SetCountNoConstructor(zenUInt _uCount)
{
	AWDelNullArray(mpData);			
	if( _uCount )	mpData	= (TType*) AWNew(mpAllocator) zenU8[sizeof(TType)*_uCount];
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
