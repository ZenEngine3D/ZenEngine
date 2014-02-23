
namespace zen { namespace awtype {

template<class TType, GrowthPolicyFunction TGrowthPolicy>
awArrayDynamic<TType, TGrowthPolicy>::awArrayDynamic(CMem::Allocator* _pAllocator=NULL)		
: awArrayBase(_pAllocator)
, muCountReserved(0)
, muCountReservedMin(0)
{
}

template<class TType, GrowthPolicyFunction TGrowthPolicy>
awArrayDynamic<TType, TGrowthPolicy>::awArrayDynamic(awU32 _uCount, CMem::Allocator* _pAllocator=NULL)
: awArrayBase(_pAllocator)
, muCountReserved(0)
, muCountReservedMin(0)
{
	muCount = _uCount;
	Grow(muCount); 
}	

template<class TType, GrowthPolicyFunction TGrowthPolicy>
awArrayDynamic<TType, TGrowthPolicy>::awArrayDynamic(const TType* _pCopy, awUInt _uCount, awUInt _uExtraCount=0, CMem::Allocator* _pAllocator=NULL)
: awArrayBase(_pAllocator)
, muCountReserved(0)
, muCountReservedMin(0)
{		
	Grow(_uCount+_uExtraCount);
	muCount	= _uCount;
	if( AWSupportsMemCopy<TType>::value )
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
awArrayDynamic<TType, TGrowthPolicy>::awArrayDynamic(const awArrayDynamic& _Copy, awUInt _uExtraCount=0)
: awArrayBase(_Copy.mpAllocator)
, muCountReserved(0)
, muCountReservedMin(0)
{			
	Grow(_Copy.Count()+_uExtraCount);
	muCount					= _Copy.Count();
	const TType* pItemSrc	= _Copy.First();
	if( AWSupportsMemCopy<TType>::value )
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
awArrayDynamic<TType, TGrowthPolicy>::~awArrayDynamic()
{
	AWDelNullArray(mpData);
}
		
template<class TType, GrowthPolicyFunction TGrowthPolicy>
awUInt awArrayDynamic<TType, TGrowthPolicy>::SetCount(awUInt _uCount)
{			
	if( _uCount > muCountReserved )	{Grow(_uCount); muCount = _uCount;}
	else if( _uCount < muCount )	{muCount = _uCount; Shrink();}
	else							{muCount = _uCount;}
	return muCount;
}

template<class TType, GrowthPolicyFunction TGrowthPolicy>
awUInt awArrayDynamic<TType, TGrowthPolicy>::SetCountNoConstructor(awUInt _uCount)
{
	if( _uCount > muCountReserved )	{GrowNoConstructor(_uCount); muCount = _uCount;}
	else if( _uCount < muCount )	{muCount = _uCount; Shrink();}
	else							{muCount = _uCount;}
	return muCount;
}

template<class TType, GrowthPolicyFunction TGrowthPolicy>
void awArrayDynamic<TType, TGrowthPolicy>::operator+=( const awArrayBase<TType>& _ArrayAdd )
{
	if( muCount+_ArrayAdd.Count() > muCountReserved )
		Grow(muCount+_ArrayAdd.Count());
	zenMem::Copy( &mpData[muCount], _ArrayAdd.First(), _ArrayAdd.Count()*sizeof(TType) );
	muCount += _ArrayAdd.Count();
}

template<class TType, GrowthPolicyFunction TGrowthPolicy>
void awArrayDynamic<TType, TGrowthPolicy>::Append(const TType& _Copy)
{
	if(muCount+1>=muCountReserved)
		Grow(muCount+1);
	mpData[muCount++] = _Copy;
}

template<class TType, GrowthPolicyFunction TGrowthPolicy>
void awArrayDynamic<TType, TGrowthPolicy>::Append(const TType* _Copy, awUInt _uCount)
{			
	if(muCount+_uCount>=muCountReserved)
		Grow(muCount+_uCount);

	zenMem::Copy(&mpData[muCount], _Copy, _uCount*sizeof(TType) );
	muCount += _uCount;
}

template<class TType, GrowthPolicyFunction TGrowthPolicy>
void awArrayDynamic<TType, TGrowthPolicy>::Reserve(awUInt _uCount)
{
	muCountReservedMin = _uCount;
	if(_uCount>muCountReserved)
		Grow(_uCount);
}

template<class TType, GrowthPolicyFunction TGrowthPolicy>
TType awArrayDynamic<TType, TGrowthPolicy>::Pop()
{
	AWAssert( muCount > 0);
	TType value = mpData[--muCount];
	Shrink();			
	return value;
}

template<class TType, GrowthPolicyFunction TGrowthPolicy>
awUInt awArrayDynamic<TType, TGrowthPolicy>::ReservedCount() const	
{
	return muCountReserved;
}

template<class TType, GrowthPolicyFunction TGrowthPolicy>
awUInt awArrayDynamic<TType, TGrowthPolicy>::ReservedSize() const	
{
	return muCountReserved*sizeof(TType);
}
		
//==================================================================================================		
//! @details	
//==================================================================================================
template<class TType, GrowthPolicyFunction TGrowthPolicy>
void awArrayDynamic<TType, TGrowthPolicy>::Grow( awUInt _auCountNeeded )
{
	awUInt uNewCount	= TGrowthPolicy(muCountReserved, _auCountNeeded, sizeof(TType) );
	AWAssertMsg(uNewCount>=_auCountNeeded, "Growth function returned less item count than needed");
	TType* pNewData = AWNew(mpAllocator) TType[uNewCount];
	if( mpData )
	{
		zenMem::Copy(pNewData, mpData, muCount*sizeof(TType) );
		AWDelNullArray(mpData);
	}			
	mpData			= pNewData;
	muCountReserved	= uNewCount;			
}

//==================================================================================================
//! @details	
//==================================================================================================
template<class TType, GrowthPolicyFunction TGrowthPolicy>
void awArrayDynamic<TType, TGrowthPolicy>::GrowNoConstructor( awUInt _auCountNeeded )
{
	awUInt uNewCount	= TGrowthPolicy(muCountReserved, _auCountNeeded, sizeof(TType) );
	AWAssertMsg(uNewCount>=_auCountNeeded, "Growth function returned less item count than needed");
	TType* pNewData = reinterpret_cast<TType*>( AWNew(mpAllocator) awU8[sizeof(TType)*uNewCount] );
	zenMem::Copy(pNewData, mpData, muCount*sizeof(TType) );
	AWDelNullArray(mpData);
	mpData			= pNewData;
	muCountReserved	= uNewCount;			
}
		
//==================================================================================================
//! @details	Called when we remove item from array. Has to check if we have to much memory still 
//!				Reserved (by using the growth function)
//==================================================================================================
template<class TType, GrowthPolicyFunction TGrowthPolicy>
void awArrayDynamic<TType, TGrowthPolicy>::Shrink( )
{
	awUInt uWantedCount = TGrowthPolicy(muCountReserved, muCount, sizeof(TType) );
	if( uWantedCount < muCountReserved && uWantedCount > muCountReservedMin)
	{
		TType* pNewData = AWNew(mpAllocator) TType[uWantedCount];
		zenMem::Copy(pNewData, mpData, muCount*sizeof(TType) );
		AWDelNullArray(mpData);
		mpData			= pNewData;
		muCountReserved	= uWantedCount;
	}
}
	

} } //namespace zen, Type

