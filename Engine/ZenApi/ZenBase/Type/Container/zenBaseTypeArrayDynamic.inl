
namespace zen { namespace zenType {

template<class TType, GrowthPolicyFunction TGrowthPolicy>
zArrayDynamic<TType, TGrowthPolicy>::zArrayDynamic()
: zArrayBase()
, muCountReserved(0)
, muCountReservedMin(0)
{
}

template<class TType, GrowthPolicyFunction TGrowthPolicy>
zArrayDynamic<TType, TGrowthPolicy>::zArrayDynamic(zU32 _uCount)
: zArrayBase()
, muCountReserved(0)
, muCountReservedMin(0)
{
	muCount = _uCount;
	Grow(muCount); 
}	

template<class TType, GrowthPolicyFunction TGrowthPolicy>
zArrayDynamic<TType, TGrowthPolicy>::zArrayDynamic(const TType* _pCopy, zUInt _uCount, zUInt _uExtraCount=0)
: zArrayBase()
, muCountReserved(0)
, muCountReservedMin(0)
{		
	Grow(_uCount+_uExtraCount);
	muCount	= _uCount;
	if( ZenSupportsMemCopy<TType>::value )
	{
		zenMem::Copy(mpData, _pCopy, sizeof(TType)*_uCount);
	}
	else				
	{				
		TType* pItemCur	= mpData;
		TType* pItemEnd	= mpData+_uCount;
		while( pItemCur < pItemEnd )
			*pItemCur++ = *_pCopy++;
	}
}

template<class TType, GrowthPolicyFunction TGrowthPolicy>
zArrayDynamic<TType, TGrowthPolicy>::zArrayDynamic(const zArrayDynamic& _Copy, zUInt _uExtraCount=0)
: zArrayBase()
, muCountReserved(0)
, muCountReservedMin(0)
{			
	Grow(_Copy.Count()+_uExtraCount);
	muCount					= _Copy.Count();
	const TType* pItemSrc	= _Copy.First();
	if( ZenSupportsMemCopy<TType>::value )
	{
		zenMem::Copy(mpData, pItemSrc, sizeof(TType)*_Copy.Count());
	}
	else				
	{					
		TType* pItemCur	= mpData;
		TType* pItemEnd	= mpData+muCount;
		while( pItemCur < pItemEnd )
			*pItemCur++ = *pItemSrc++;
	}
}

template<class TType, GrowthPolicyFunction TGrowthPolicy>
zArrayDynamic<TType, TGrowthPolicy>::~zArrayDynamic()
{
	zenDelNullArray(mpData);
}
		
template<class TType, GrowthPolicyFunction TGrowthPolicy>
zUInt zArrayDynamic<TType, TGrowthPolicy>::SetCount(zUInt _uCount)
{			
	if( _uCount > muCountReserved )	{Grow(_uCount); muCount = _uCount;}
	else if( _uCount < muCount )	{muCount = _uCount; Shrink();}
	else							{muCount = _uCount;}
	return muCount;
}

template<class TType, GrowthPolicyFunction TGrowthPolicy>
zUInt zArrayDynamic<TType, TGrowthPolicy>::SetCountNoConstructor(zUInt _uCount)
{
	if( _uCount > muCountReserved )	{GrowNoConstructor(_uCount); muCount = _uCount;}
	else if( _uCount < muCount )	{muCount = _uCount; Shrink();}
	else							{muCount = _uCount;}
	return muCount;
}

template<class TType, GrowthPolicyFunction TGrowthPolicy>
void zArrayDynamic<TType, TGrowthPolicy>::operator+=( const zArrayBase<TType>& _ArrayAdd )
{
	if( muCount+_ArrayAdd.Count() > muCountReserved )
		Grow(muCount+_ArrayAdd.Count());
	zenMem::Copy( &mpData[muCount], _ArrayAdd.First(), _ArrayAdd.Count()*sizeof(TType) );
	muCount += _ArrayAdd.Count();
}

template<class TType, GrowthPolicyFunction TGrowthPolicy>
void zArrayDynamic<TType, TGrowthPolicy>::Push(const TType& _Copy)
{
	if(muCount+1>=muCountReserved)
		Grow(muCount+1);
	mpData[muCount++] = _Copy;
}

template<class TType, GrowthPolicyFunction TGrowthPolicy>
void zArrayDynamic<TType, TGrowthPolicy>::Push(const TType* _Copy, zUInt _uCount)
{			
	if(muCount+_uCount>=muCountReserved)
		Grow(muCount+_uCount);

	zenMem::Copy(&mpData[muCount], _Copy, _uCount*sizeof(TType) );
	muCount += _uCount;
}

template<class TType, GrowthPolicyFunction TGrowthPolicy>
void zArrayDynamic<TType, TGrowthPolicy>::Reserve(zUInt _uCount)
{
	muCountReservedMin = _uCount;
	if(_uCount>muCountReserved)
		Grow(_uCount);
}

template<class TType, GrowthPolicyFunction TGrowthPolicy>
TType zArrayDynamic<TType, TGrowthPolicy>::Pop()
{
	ZENAssert( muCount > 0);
	TType value = mpData[--muCount];
	Shrink();			
	return value;
}

template<class TType, GrowthPolicyFunction TGrowthPolicy>
void zArrayDynamic<TType, TGrowthPolicy>::RemoveSwap(zUInt _uIndex)
{
	ZENAssert( _uIndex < muCount );
	mpData[_uIndex] = mpData[--muCount];
	Shrink();
}

template<class TType, GrowthPolicyFunction TGrowthPolicy>
void zArrayDynamic<TType, TGrowthPolicy>::RemoveSwap(const TType& _Item)
{
	zUInt uIndex(0);
	while( uIndex < muCount )
	{
		if( mpData[uIndex] == _Item )
		{
			RemoveSwap( uIndex );
			break;
		}
		++uIndex;
	}
}

template<class TType, GrowthPolicyFunction TGrowthPolicy>
zUInt zArrayDynamic<TType, TGrowthPolicy>::ReservedCount() const	
{
	return muCountReserved;
}

template<class TType, GrowthPolicyFunction TGrowthPolicy>
zUInt zArrayDynamic<TType, TGrowthPolicy>::ReservedSize() const	
{
	return muCountReserved*sizeof(TType);
}
		
//==================================================================================================		
//! @details	
//==================================================================================================
template<class TType, GrowthPolicyFunction TGrowthPolicy>
void zArrayDynamic<TType, TGrowthPolicy>::Grow( zUInt _auCountNeeded )
{
	zUInt uNewCount	= TGrowthPolicy(muCountReserved, _auCountNeeded, sizeof(TType) );
	ZENAssertMsg(uNewCount>=_auCountNeeded, "Growth function returned less item count than needed");
	TType* pNewData = zenNewDefault TType[uNewCount];
	if( mpData )
	{
		zenMem::Copy(pNewData, mpData, muCount*sizeof(TType) );
		zenDelNullArray(mpData);
	}			
	mpData			= pNewData;
	muCountReserved	= uNewCount;			
}

//==================================================================================================
//! @details	
//==================================================================================================
template<class TType, GrowthPolicyFunction TGrowthPolicy>
void zArrayDynamic<TType, TGrowthPolicy>::GrowNoConstructor( zUInt _auCountNeeded )
{
	zUInt uNewCount	= TGrowthPolicy(muCountReserved, _auCountNeeded, sizeof(TType) );
	ZENAssertMsg(uNewCount>=_auCountNeeded, "Growth function returned less item count than needed");
	TType* pNewData = reinterpret_cast<TType*>( zenNewDefault zU8[sizeof(TType)*uNewCount] );
	zenMem::Copy(pNewData, mpData, muCount*sizeof(TType) );
	zenDelNullArray(mpData);
	mpData			= pNewData;
	muCountReserved	= uNewCount;			
}
		
//==================================================================================================
//! @details	Called when we remove item from array. Has to check if we have to much memory still 
//!				Reserved (by using the growth function)
//==================================================================================================
template<class TType, GrowthPolicyFunction TGrowthPolicy>
void zArrayDynamic<TType, TGrowthPolicy>::Shrink( )
{
	zUInt uWantedCount = TGrowthPolicy(muCountReserved, muCount, sizeof(TType) );
	if( uWantedCount < muCountReserved && uWantedCount > muCountReservedMin)
	{
		TType* pNewData = zenNewDefault TType[uWantedCount];
		zenMem::Copy(pNewData, mpData, muCount*sizeof(TType) );
		zenDelNullArray(mpData);
		mpData			= pNewData;
		muCountReserved	= uWantedCount;
	}
}
	

} } //namespace zen, Type

