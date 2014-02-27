
namespace zen { namespace zenType {

template<class TType, GrowthPolicyFunction TGrowthPolicy>
zenArrayDynamic<TType, TGrowthPolicy>::zenArrayDynamic()
: zenArrayBase()
, muCountReserved(0)
, muCountReservedMin(0)
{
}

template<class TType, GrowthPolicyFunction TGrowthPolicy>
zenArrayDynamic<TType, TGrowthPolicy>::zenArrayDynamic(zenU32 _uCount)
: zenArrayBase()
, muCountReserved(0)
, muCountReservedMin(0)
{
	muCount = _uCount;
	Grow(muCount); 
}	

template<class TType, GrowthPolicyFunction TGrowthPolicy>
zenArrayDynamic<TType, TGrowthPolicy>::zenArrayDynamic(const TType* _pCopy, zenUInt _uCount, zenUInt _uExtraCount=0)
: zenArrayBase()
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
zenArrayDynamic<TType, TGrowthPolicy>::zenArrayDynamic(const zenArrayDynamic& _Copy, zenUInt _uExtraCount=0)
: zenArrayBase()
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
zenArrayDynamic<TType, TGrowthPolicy>::~zenArrayDynamic()
{
	zenDelNullArray(mpData);
}
		
template<class TType, GrowthPolicyFunction TGrowthPolicy>
zenUInt zenArrayDynamic<TType, TGrowthPolicy>::SetCount(zenUInt _uCount)
{			
	if( _uCount > muCountReserved )	{Grow(_uCount); muCount = _uCount;}
	else if( _uCount < muCount )	{muCount = _uCount; Shrink();}
	else							{muCount = _uCount;}
	return muCount;
}

template<class TType, GrowthPolicyFunction TGrowthPolicy>
zenUInt zenArrayDynamic<TType, TGrowthPolicy>::SetCountNoConstructor(zenUInt _uCount)
{
	if( _uCount > muCountReserved )	{GrowNoConstructor(_uCount); muCount = _uCount;}
	else if( _uCount < muCount )	{muCount = _uCount; Shrink();}
	else							{muCount = _uCount;}
	return muCount;
}

template<class TType, GrowthPolicyFunction TGrowthPolicy>
void zenArrayDynamic<TType, TGrowthPolicy>::operator+=( const zenArrayBase<TType>& _ArrayAdd )
{
	if( muCount+_ArrayAdd.Count() > muCountReserved )
		Grow(muCount+_ArrayAdd.Count());
	zenMem::Copy( &mpData[muCount], _ArrayAdd.First(), _ArrayAdd.Count()*sizeof(TType) );
	muCount += _ArrayAdd.Count();
}

template<class TType, GrowthPolicyFunction TGrowthPolicy>
void zenArrayDynamic<TType, TGrowthPolicy>::Append(const TType& _Copy)
{
	if(muCount+1>=muCountReserved)
		Grow(muCount+1);
	mpData[muCount++] = _Copy;
}

template<class TType, GrowthPolicyFunction TGrowthPolicy>
void zenArrayDynamic<TType, TGrowthPolicy>::Append(const TType* _Copy, zenUInt _uCount)
{			
	if(muCount+_uCount>=muCountReserved)
		Grow(muCount+_uCount);

	zenMem::Copy(&mpData[muCount], _Copy, _uCount*sizeof(TType) );
	muCount += _uCount;
}

template<class TType, GrowthPolicyFunction TGrowthPolicy>
void zenArrayDynamic<TType, TGrowthPolicy>::Reserve(zenUInt _uCount)
{
	muCountReservedMin = _uCount;
	if(_uCount>muCountReserved)
		Grow(_uCount);
}

template<class TType, GrowthPolicyFunction TGrowthPolicy>
TType zenArrayDynamic<TType, TGrowthPolicy>::Pop()
{
	ZENAssert( muCount > 0);
	TType value = mpData[--muCount];
	Shrink();			
	return value;
}

template<class TType, GrowthPolicyFunction TGrowthPolicy>
zenUInt zenArrayDynamic<TType, TGrowthPolicy>::ReservedCount() const	
{
	return muCountReserved;
}

template<class TType, GrowthPolicyFunction TGrowthPolicy>
zenUInt zenArrayDynamic<TType, TGrowthPolicy>::ReservedSize() const	
{
	return muCountReserved*sizeof(TType);
}
		
//==================================================================================================		
//! @details	
//==================================================================================================
template<class TType, GrowthPolicyFunction TGrowthPolicy>
void zenArrayDynamic<TType, TGrowthPolicy>::Grow( zenUInt _auCountNeeded )
{
	zenUInt uNewCount	= TGrowthPolicy(muCountReserved, _auCountNeeded, sizeof(TType) );
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
void zenArrayDynamic<TType, TGrowthPolicy>::GrowNoConstructor( zenUInt _auCountNeeded )
{
	zenUInt uNewCount	= TGrowthPolicy(muCountReserved, _auCountNeeded, sizeof(TType) );
	ZENAssertMsg(uNewCount>=_auCountNeeded, "Growth function returned less item count than needed");
	TType* pNewData = reinterpret_cast<TType*>( zenNewDefault zenU8[sizeof(TType)*uNewCount] );
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
void zenArrayDynamic<TType, TGrowthPolicy>::Shrink( )
{
	zenUInt uWantedCount = TGrowthPolicy(muCountReserved, muCount, sizeof(TType) );
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

