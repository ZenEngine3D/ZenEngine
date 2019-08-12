namespace zen { namespace zenType {

template<class TType, unsigned long TCount>
zArrayFixed<TType, TCount>::zArrayFixed()
{
	mpData	= mDataArray;
	mCount	= TCount;
}

template<class TType, unsigned long TCount>
zArrayFixed<TType, TCount>::zArrayFixed(const zArray<TType>& inCopy)
{ 
	Super::Copy<TType>(inCopy.front(), inCopy.size());
}

template<class TType, unsigned long TCount>
zArrayFixed<TType, TCount>::zArrayFixed(const zArrayFixed<TType,TCount>& inCopy)
{ 
	Super::Copy<TType>(inCopy.front(), inCopy.size());
}

template<class TType, unsigned long TCount>
zArrayFixed<TType, TCount>::zArrayFixed(std::initializer_list<TType> inEntries)
{
	zenAssert(inEntries.size() <= TCount)
	mpData	= mDataArray;
	mCount	= TCount;
	Super::Copy(inEntries.begin(), inEntries.size());
}
	
template<class TType, unsigned long TCount>
typename zArrayFixed<TType, TCount>& zArrayFixed<TType, TCount>::operator=(const zArray<TType>& inCopy)
{ 
	Super::Copy<TType>(inCopy.front(), inCopy.size());
	return *this;
}
template<class TType, unsigned long TCount>
template<unsigned long TCountCopy>
typename zArrayFixed<TType, TCount>& zArrayFixed<TType, TCount>::operator=(const zArrayFixed<TType,TCountCopy>& inCopy)
{ 
	Super::Copy<TType>(inCopy.front(), inCopy.size());
	return *this;
}	
} } //namespace zen, Type
