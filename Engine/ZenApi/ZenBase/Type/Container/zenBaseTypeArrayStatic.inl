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
	mpData	= zenNew TType[muCount];			
}

template<class TType>
zArrayStatic<TType>::zArrayStatic(const TType* _pCopy, zUInt _uCount, zUInt _uExtraCount)
: zArrayBase()
{		
	muCount			= _uCount+_uExtraCount;
	mpData			= zenNew TType[muCount];
	zenMem::Copy(mpData, _pCopy, _uCount);
}	

template<class TType>
zArrayStatic<TType>::zArrayStatic(const zArrayStatic& _Copy, zUInt _uExtraCount)
{					
	muCount					= _Copy.Count()+_uExtraCount;
	mpData					= zenNew TType[muCount];		
	const TType* pItemSrc	= _Copy.First();
	zenMem::Copy(mpData, pItemSrc, muCount);
}

template<class TType>
zArrayStatic<TType>::zArrayStatic(std::initializer_list<TType> _Entries)
{	
	muCount					= (zUInt)_Entries.size();
	mpData					= zenNew TType[muCount];		
	const TType* pItemSrc	= _Entries.begin();
	zenMem::Copy(mpData, pItemSrc, muCount);
}		

template<class TType>
zArrayStatic<TType>::~zArrayStatic()
{
	zenDelArrayNullptr(mpData);
}
		
template<class TType>
zUInt zArrayStatic<TType>::SetCount(zUInt _uCount)
{
	zenDelArrayNullptr(mpData);
	if( _uCount )	mpData = zenNew TType[_uCount];
	else			mpData = nullptr;
	muCount	= _uCount;
	return muCount;
}

template<class TType>	
zUInt zArrayStatic<TType>::SetCountNoConstructor(zUInt _uCount)
{
	zenDelArrayNullptr(mpData);			
	if( _uCount )	mpData	= (TType*) zenNew zU8[sizeof(TType)*_uCount];
	else			mpData	= nullptr;
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
