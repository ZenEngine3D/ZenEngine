
namespace zen { namespace zenType {

template<class TType, GrowthPolicyFunction TGrowthPolicy, size_t TAlign>
zArrayDynamic<TType, TGrowthPolicy, TAlign>::zArrayDynamic()
: zArrayBase()
, muCountReserved(0)
, muCountReservedMin(0)
{
}

template<class TType, GrowthPolicyFunction TGrowthPolicy, size_t TAlign>
zArrayDynamic<TType, TGrowthPolicy, TAlign>::zArrayDynamic(zU32 _uCount)
: zArrayBase()
, muCountReserved(0)
, muCountReservedMin(0)
{
	muCount = _uCount;
	Grow(muCount); 
}	

template<class TType, GrowthPolicyFunction TGrowthPolicy, size_t TAlign>
zArrayDynamic<TType, TGrowthPolicy, TAlign>::zArrayDynamic(std::initializer_list<TType> _Entries)
: zArrayBase()
, muCountReserved(0)
, muCountReservedMin(0)
{
	muCount					= (zUInt)_Entries.size();
	const TType* pItemSrc	= _Entries.begin();
	Grow(muCount);	
	zenMem::Copy(mpData, pItemSrc, muCount);	
}

template<class TType, GrowthPolicyFunction TGrowthPolicy, size_t TAlign>
zArrayDynamic<TType, TGrowthPolicy, TAlign>::zArrayDynamic(const TType* _pCopy, zUInt _uCount, zUInt _uExtraCount=0)
: zArrayBase()
, muCountReserved(0)
, muCountReservedMin(0)
{		
	Grow(_uCount+_uExtraCount);
	muCount	= _uCount;
	zenMem::Copy(mpData, _pCopy, _uCount);
}

template<class TType, GrowthPolicyFunction TGrowthPolicy, size_t TAlign>
zArrayDynamic<TType, TGrowthPolicy, TAlign>::zArrayDynamic(const zArrayDynamic& _Copy, zUInt _uExtraCount=0)
: zArrayBase()
, muCountReserved(0)
, muCountReservedMin(0)
{			
	Grow(_Copy.Count()+_uExtraCount);
	muCount					= _Copy.Count();
	const TType* pItemSrc	= _Copy.First();	
	zenMem::Copy(mpData, pItemSrc, _Copy.Count());
}

template<class TType, GrowthPolicyFunction TGrowthPolicy, size_t TAlign>
zArrayDynamic<TType, TGrowthPolicy, TAlign>::~zArrayDynamic()
{
	zenDelnullptrArray(mpData);
}
		
template<class TType, GrowthPolicyFunction TGrowthPolicy, size_t TAlign>
zUInt zArrayDynamic<TType, TGrowthPolicy, TAlign>::SetCount(zUInt _uCount)
{			
	if( _uCount > muCountReserved )	{Grow(_uCount); muCount = _uCount;}
	else if( _uCount < muCount )	{muCount = _uCount; Shrink();}
	else							{muCount = _uCount;}
	return muCount;
}

template<class TType, GrowthPolicyFunction TGrowthPolicy, size_t TAlign>
zUInt zArrayDynamic<TType, TGrowthPolicy, TAlign>::IncCount(zUInt _uCountAddtional)
{
	return SetCount( muCount + _uCountAddtional);
}

template<class TType, GrowthPolicyFunction TGrowthPolicy, size_t TAlign>
zUInt zArrayDynamic<TType, TGrowthPolicy, TAlign>::SetCountNoConstructor(zUInt _uCount)
{
	if( _uCount > muCountReserved )	{GrowNoConstructor(_uCount); muCount = _uCount;}
	else if( _uCount < muCount )	{muCount = _uCount; Shrink();}
	else							{muCount = _uCount;}
	return muCount;
}

template<class TType, GrowthPolicyFunction TGrowthPolicy, size_t TAlign>
void zArrayDynamic<TType, TGrowthPolicy, TAlign>::operator+=( const zArrayBase<TType>& _ArrayAdd )
{
	if( muCount+_ArrayAdd.Count() > muCountReserved )
		Grow(muCount+_ArrayAdd.Count());
	
	zenMem::Copy( &mpData[muCount], _ArrayAdd.First(), _ArrayAdd.Count() );	
	muCount += _ArrayAdd.Count();
}

template<class TType, GrowthPolicyFunction TGrowthPolicy, size_t TAlign>
void zArrayDynamic<TType, TGrowthPolicy, TAlign>::Push(const TType& _Copy)
{
	if(muCount+1>=muCountReserved)
		Grow(muCount+1);
	mpData[muCount++] = _Copy;
}

template<class TType, GrowthPolicyFunction TGrowthPolicy, size_t TAlign>
void zArrayDynamic<TType, TGrowthPolicy, TAlign>::Push(const TType* _Copy, zUInt _uCount)
{			
	if(muCount+_uCount>=muCountReserved)
		Grow(muCount+_uCount);

	zenMem::Copy(&mpData[muCount], _Copy, _uCount*sizeof(TType) );
	muCount += _uCount;
}

template<class TType, GrowthPolicyFunction TGrowthPolicy, size_t TAlign>
void zArrayDynamic<TType, TGrowthPolicy, TAlign>::Reserve(zUInt _uCount)
{
	muCountReservedMin = _uCount;
	if(_uCount>muCountReserved)
		Grow(_uCount);
}

template<class TType, GrowthPolicyFunction TGrowthPolicy, size_t TAlign>
TType zArrayDynamic<TType, TGrowthPolicy, TAlign>::Pop()
{
	zenAssert( muCount > 0);
	TType value = mpData[--muCount];
	Shrink();			
	return value;
}

template<class TType, GrowthPolicyFunction TGrowthPolicy, size_t TAlign>
void zArrayDynamic<TType, TGrowthPolicy, TAlign>::RemoveSwap(zUInt _uIndex)
{
	zenAssert( _uIndex < muCount );
	mpData[_uIndex] = mpData[--muCount];
	Shrink();
}

template<class TType, GrowthPolicyFunction TGrowthPolicy, size_t TAlign>
void zArrayDynamic<TType, TGrowthPolicy, TAlign>::RemoveSwap(const TType& _Item)
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

template<class TType, GrowthPolicyFunction TGrowthPolicy, size_t TAlign>
zUInt zArrayDynamic<TType, TGrowthPolicy, TAlign>::ReservedCount() const	
{
	return muCountReserved;
}

template<class TType, GrowthPolicyFunction TGrowthPolicy, size_t TAlign>
zUInt zArrayDynamic<TType, TGrowthPolicy, TAlign>::ReservedSize() const	
{
	return muCountReserved*sizeof(TType);
}
		
//==================================================================================================		
//! @details	
//==================================================================================================
template<class TType, GrowthPolicyFunction TGrowthPolicy, size_t TAlign>
void zArrayDynamic<TType, TGrowthPolicy, TAlign>::Grow( zUInt _auCountNeeded )
{
	zUInt uNewCount	= TGrowthPolicy(muCountReserved, _auCountNeeded, sizeof(TType) );
	zenAssertMsg(uNewCount>=_auCountNeeded, "Growth function returned less item count than needed");
	TType* pNewData = zenNewDefault TType[uNewCount];
	zenMem::Copy(pNewData, mpData, muCount);
	zenDelnullptrArray(mpData);	
	mpData			= pNewData;
	muCountReserved	= uNewCount;			
}

//==================================================================================================
//! @details	
//==================================================================================================
template<class TType, GrowthPolicyFunction TGrowthPolicy, size_t TAlign>
void zArrayDynamic<TType, TGrowthPolicy, TAlign>::GrowNoConstructor( zUInt _auCountNeeded )
{
	zUInt uNewCount	= TGrowthPolicy(muCountReserved, _auCountNeeded, sizeof(TType) );
	zenAssertMsg(uNewCount>=_auCountNeeded, "Growth function returned less item count than needed");
	TType* pNewData = reinterpret_cast<TType*>( zenNewDefault zU8[sizeof(TType)*uNewCount] );	
	zenMem::Copy(pNewData, mpData, muCount );
	zenDelnullptrArray(mpData);

	mpData			= pNewData;
	muCountReserved	= uNewCount;			
}
		
//==================================================================================================
//! @details	Called when we remove item from array. Has to check if we have to much memory still 
//!				Reserved (by using the growth function)
//==================================================================================================
template<class TType, GrowthPolicyFunction TGrowthPolicy, size_t TAlign>
void zArrayDynamic<TType, TGrowthPolicy, TAlign>::Shrink( )
{
	zUInt uWantedCount = TGrowthPolicy(muCountReserved, muCount, sizeof(TType) );
	if( uWantedCount < muCountReserved && uWantedCount > muCountReservedMin)
	{
		TType* pNewData = zenNewDefault TType[uWantedCount];
		zenMem::Copy(pNewData, mpData, muCount);
		zenDelnullptrArray(mpData);
		mpData			= pNewData;
		muCountReserved	= uWantedCount;
	}
}

} } //namespace zen, Type

