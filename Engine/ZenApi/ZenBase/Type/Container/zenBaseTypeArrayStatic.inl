namespace zen { namespace zenType {

template<class TType, size_t TAlign>
zArrayStatic<TType,TAlign>::zArrayStatic()
: zArrayBase()
{
}

template<class TType, size_t TAlign>
zArrayStatic<TType,TAlign>::zArrayStatic(zUInt _uCount)
: zArrayBase()
{
	muCount	= _uCount;
	mpData	= zenNewDefaultAlign(TAlign) TType[muCount];			
}

template<class TType, size_t TAlign>
zArrayStatic<TType,TAlign>::zArrayStatic(const TType* _pCopy, zUInt _uCount, zUInt _uExtraCount=0)
: zArrayBase()
{		
	muCount			= _uCount+_uExtraCount;
	mpData			= zenNewDefaultAlign(TAlign) TType[muCount];
	zenMem::Copy(mpData, _pCopy, _uCount);
}	

template<class TType, size_t TAlign>
zArrayStatic<TType,TAlign>::zArrayStatic(const zArrayStatic& _Copy, zUInt _uExtraCount=0)
{					
	muCount					= _Copy.Count()+_uExtraCount;
	mpData					= zenNewDefaultAlign(TAlign) TType[muCount];		
	const TType* pItemSrc	= _Copy.First();
	zenMem::Copy(mpData, pItemSrc, muCount);
}

template<class TType, size_t TAlign>
zArrayStatic<TType,TAlign>::zArrayStatic(std::initializer_list<TType> _Entries)
{	
	muCount					= (zUInt)_Entries.size();
	mpData					= zenNewDefaultAlign(TAlign) TType[muCount];		
	const TType* pItemSrc	= _Entries.begin();
	zenMem::Copy(mpData, pItemSrc, muCount);
}		

template<class TType, size_t TAlign>
zArrayStatic<TType,TAlign>::~zArrayStatic()
{
	zenDelnullptrArray(mpData);
}
		
template<class TType, size_t TAlign>
zUInt zArrayStatic<TType,TAlign>::SetCount(zUInt _uCount)
{
	zenDelnullptrArray(mpData);
	if( _uCount )	mpData = zenNewDefaultAlign(TAlign) TType[_uCount];
	else			mpData = nullptr;
	muCount	= _uCount;
	return muCount;
}

template<class TType, size_t TAlign>	
zUInt zArrayStatic<TType,TAlign>::SetCountNoConstructor(zUInt _uCount)
{
	zenDelnullptrArray(mpData);			
	if( _uCount )	mpData	= (TType*) zenNewDefaultAlign(TAlign) zU8[sizeof(TType)*_uCount];
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
template<class TType, size_t TAlign>
void zArrayStatic<TType,TAlign>::Swap(zArrayStatic<TType,TAlign>& _Copy)
{
	TType*	pCopyData(_Copy.mpData);
	zUInt	uCopyCount(_Copy.muCount);
	_Copy.mpData	= mpData;
	_Copy.muCount	= muCount;
	mpData			= pCopyData;
	muCount			= uCopyCount;
}


	
} } //namespace zen, Type
