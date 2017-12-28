namespace zen { namespace zenType {

//! @todo Multihread and move this elsewhere
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

template<class TType>
zArrayBase<TType>::zArrayBase()
: mpData(nullptr)
, muCount(0)
{
}

template<class TType>
void zArrayBase<TType>::Sort()
{
	QuicksortAsc(mpData, &mpData[muCount-1]);	
}

template<class TType>
void zArrayBase<TType>::Clear()
{
	SetCount(0);
}

template<class TType>
TType* zArrayBase<TType>::First()
{
	return muCount ? mpData : nullptr;
}

template<class TType>
const TType* zArrayBase<TType>::First()const
{
	return muCount ? mpData : nullptr;
}

template<class TType>
TType* zArrayBase<TType>::Last()
{
	return muCount ? (mpData + muCount - 1) : nullptr;
}

template<class TType>
const TType* zArrayBase<TType>::Last()const	
{
	return muCount ? (mpData + muCount - 1) : nullptr;
}

template<class TType>
zUInt zArrayBase<TType>::Count()const
{
	return muCount;
}

template<class TType>
bool zArrayBase<TType>::IsEmpty()const
{
	return muCount == 0;
}

template<class TType>
zUInt zArrayBase<TType>::SizeMem()const
{
	return muCount*sizeof(TType);
}	

template<class TType>
zUInt zArrayBase<TType>::SizeItem()const			
{
	return sizeof(TType);
}

template<class TType>
void zArrayBase<TType>::SetAll(const TType& _Value) 
{
	SetRange(_Value);
}

template<class TType>
TType& zArrayBase<TType>::operator[](zInt _sIndex)
{
	zenAssert( (_sIndex>=0 && zUInt(_sIndex)<muCount) || (_sIndex<0 && zUInt(-_sIndex) <= muCount) );
	return _sIndex >= 0 ? mpData[_sIndex] : mpData[muCount+_sIndex];
}

template<class TType>
const TType& zArrayBase<TType>::operator[](zInt _sIndex)const
{
	zenAssert( (_sIndex>=0 && zUInt(_sIndex)<muCount) || (_sIndex<0 && zUInt(-_sIndex) <= muCount) );
	return _sIndex >= 0 ? mpData[_sIndex] : mpData[muCount+_sIndex];
}

//==================================================================================================
//! @brief		Search for a value inside array
//! @details	
//--------------------------------------------------------------------------------------------------
//! @param _Value	- Element to look for
//! @return			- Index of element (or -1 if not found)
//==================================================================================================
template<class TType>
int zArrayBase<TType>::Find(const TType& _Value)
{
	TType* pItemCur	= mpData;
	TType* pItemEnd	= mpData+muCount;
	while( pItemCur < pItemEnd )
	{
		if( *pItemCur == _Value )	return int(pItemCur-mpData);
		else						++pItemCur;
	}
	return -1;
}

//==================================================================================================
//! @details	
//--------------------------------------------------------------------------------------------------
//! @param _Cmp		- Array to compare against
//! @return			- true of contains sames elements
//==================================================================================================
template<class TType>
bool zArrayBase<TType>::operator==(const zArrayBase& _Cmp)const
{
	TType* pItemSrcCur	= mpData;
	TType* pItemSrcEnd	= mpData+muCount;
	TType* pItemCmpCur	= _Cmp.mpData;

	if( muCount != _Cmp.muCount )
		return false;

	while( pItemSrcCur < pItemSrcEnd )
	{
		if( (*pItemSrcCur++ == *pItemCmpCur++) == false )
			return false;
	}
	return true;
}

//==================================================================================================
//! @details	
//--------------------------------------------------------------------------------------------------
//! @param _Cmp		- Array to compare against
//! @return			- true of contains sames elements
//==================================================================================================
template<class TType>
bool zArrayBase<TType>::operator!=(const zArrayBase& _Cmp)const
{
	return !(*this == _Cmp);
}

//==================================================================================================
//! @brief		Copy array value to this array
//! @details	
//--------------------------------------------------------------------------------------------------
//! @param _aCopy	- Array to copy
//! @return			- This object
//==================================================================================================
template<class TType>
zArrayBase<TType>& zArrayBase<TType>::operator=( const zArrayBase<TType>& _aCopy )
{	
	zenAssert(&_aCopy != this);
	Copy(_aCopy.First(), _aCopy.Count() );
	return *this;
}

//==================================================================================================
//! @details	Copy an array of item into this object
//!				If the array template type supports a straight memcopy, will use this instead of 
//!				more costly one by one elements copy. But for non basic type, we do need to copy 
//!				each element individually, since their operator= might be doing some internal work
//--------------------------------------------------------------------------------------------------
//! @param _pCopy	- 1st element to copy		
//! @param _uCount	- Number of element to copy
//! @return			- Number of element after copy
//==================================================================================================
template<class TType>
zUInt zArrayBase<TType>::Copy(const TType* _pCopy, zUInt _uCount)
{	
	if( mpData != _pCopy )
	{
		Clear();
		SetCount(_uCount);
		zenMem::Copy(mpData, _pCopy, _uCount);	
	}
	return muCount;
}		

//==================================================================================================
//! @details	Allow to copy from another array of different type, as long as the type support
//!				equal assignment
//--------------------------------------------------------------------------------------------------
//! @param		_ImportArray	- Pointer to elements to copy
//! @param		_uCount			- Number of element to copy
//! @return						- Number of element after copy
//==================================================================================================
template<class TType> template<class TTypeImport>
zUInt zArrayBase<TType>::Copy(const TTypeImport* _pImportArray, zUInt _uCount )
{
	if( (void*)mpData != (void*)_pImportArray )
	{
		Clear();
		SetCount(_uCount);
		TType*				pDataDest	= mpData;
		TType*				pDataEnd	= &mpData[_uCount];
		const TTypeImport*	pDataSrc	= _pImportArray;
		while( pDataDest < pDataEnd )
			*pDataDest++ = (TType)*pDataSrc++;
	}
	return muCount;
}

//==================================================================================================
//! @details	Allow to copy from another array of different type, as long as the type support
//!				equal assignment
//--------------------------------------------------------------------------------------------------
//! @param		_ImportArray	- Array to copy
//! @return						- Number of element after copy
//==================================================================================================
template<class TType> template<class TTypeImport>
zUInt zArrayBase<TType>::Copy( const zArrayBase<TTypeImport>& _ImportArray )
{
	if( (void*)this != (void*)&_ImportArray )
	{
		Clear();
		SetCount(_ImportArray.Count());
		TType*				pDataDest	= mpData;
		TType*				pDataEnd	= &mpData[muCount];
		const TTypeImport*	pDataSrc	= _ImportArray.First();
		while( pDataDest < pDataEnd )
			*pDataDest++ = *pDataSrc++;
	}
	return muCount;
}

//==================================================================================================
//! @details	
//--------------------------------------------------------------------------------------------------
//! @param _Value	- Value to assign
//! @param _uFirst	- First index to set
//! @param _uLast	- Last index to set (clamped to array size)
//! @return			- 
//==================================================================================================
template<class TType>
void zArrayBase<TType>::SetRange(const TType& _Value, zUInt _uFirst=0, zUInt _uLast=0xFFFFFFFF)
{
	TType*				pDataDest	= mpData;
	TType*				pDataEnd	= &mpData[_uLast>muCount ? muCount : _uLast];
	while( pDataDest < pDataEnd )
		*pDataDest++ = _Value;
}


} } //namespace zen, Type
