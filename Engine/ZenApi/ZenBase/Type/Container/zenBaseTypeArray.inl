namespace zen { namespace zenType {

//! @todo 3 Multi thread and move this elsewhere
template<class TType>
void QuicksortAsc(TType* _pDataFirst, TType* _pDataLast)
{
	zInt iCount =(zInt)(_pDataLast - _pDataFirst + 1);

	// Bubblesort for last few elements
	if(iCount <= 10)
	{
		for(zInt i(0); i<iCount; ++i)
			for(zInt j(1); j<iCount-i; ++j)
				if( _pDataFirst[j-1] > _pDataFirst[j] )
					zenSwap(_pDataFirst[j-1], _pDataFirst[j]);
		return;
	}

	// Quicksort proper
	zenSwap(_pDataFirst[0], _pDataFirst[iCount/2]);	//Move pivot value at start
	TType& PivotVal		= _pDataFirst[0];
	TType* pDataLeft	= _pDataFirst+1;
	TType* pDataRight	= _pDataLast;	
	while( pDataLeft < pDataRight )
	{
		if( (*pDataLeft > PivotVal) && !(*pDataRight > PivotVal) )
			zenSwap(*pDataLeft, *pDataRight);

		pDataLeft	+= (*pDataLeft > PivotVal)	? 0 : 1;
		pDataRight	-= (*pDataRight > PivotVal)	? 1 : 0;
	}

	// Move pivot value to final position
	pDataLeft -= *pDataLeft > PivotVal ? 1 : 0;
	zenSwap(PivotVal, *pDataLeft);
	
	// Recursive sorting
	QuicksortAsc(_pDataFirst, pDataLeft-1);
	QuicksortAsc(pDataLeft+1, _pDataLast);
}

//==================================================================================================
//! @details	Copy an array of item into this object
//!				If the array template type supports a straight memcopy, will use this instead of 
//!				more costly one by one elements copy. But for non basic type, we do need to copy 
//!				each element individually, since their operator= might be doing some internal work
//--------------------------------------------------------------------------------------------------
//==================================================================================================
template<class TType> 
zArray<TType>& zArray<TType>::Copy(const TType* zenRestrict inpCopy, zUInt inCount)
{	
	zenAssert((void*)mpData != (void*)inpCopy);
	zenAssert(mCount >= inCount);
	zenMem::Copy<TType>(mpData, inpCopy, inCount);		
	return *this;
}		

//==================================================================================================
//! @details	Allow to copy from another array of different type, as long as the type support
//!				equal assignment
//--------------------------------------------------------------------------------------------------
//==================================================================================================
template<class TType> template<class TImport>
zArray<TType>& zArray<TType>::Copy(const TImport* zenRestrict inpCopy, zUInt inCount)
{
	if( typeid(TType) == typeid(TImport) )
		return Copy(inpCopy, inCount);

	zenAssert((void*)mpData != (void*)inpCopy);
	zenAssert(mCount >= inCount);
	for(zUInt i(0); i<inCount; ++i)
		mpData[i] = inpCopy[i];

	return *this;
}

//==================================================================================================
//! @details	Allow to copy from another array of different type, 
//!				as long as the type support equal assignment
//--------------------------------------------------------------------------------------------------
//==================================================================================================
template<class TType> template<class TImport>
zArray<TType>& zArray<TType>::Copy(const zArray<TImport>& zenRestrict inCopy )
{	
	return Copy(inCopy.mpData, inCopy.mCount);
}

template<class TType>
bool zArray<TType>::operator==(const zArray<TType>& inCmp)const
{
	if( inCmp.mCount != mCount )
		return false;
	if( inCmp.mpData == mpData )
		return true;

	for(int i(0); i<mCount; ++i)
	{
		if( mpData[i] != inCmp.mpData[i] )
			return false;
	}

	return true;
}

template<class TType>
void zArray<TType>::Sort()
{
	if( mpData )
		QuicksortAsc(mpData, &mpData[mCount-1]);	
}

} } //namespace zen, Type
