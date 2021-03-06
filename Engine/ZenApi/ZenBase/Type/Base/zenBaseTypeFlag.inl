namespace zen { namespace zenType {

template<typename TEnumFlag, int TEnumMax, typename TStorage> 
zFlag<TEnumFlag, TEnumMax, TStorage>::zFlag() 
: muFlags(0)
{
	zenStaticAssertMsg( sizeof(TStorage)*8>=TEnumMax, "StorageContainerTooSmallForMaxValue" );
};
/*
template<typename TEnumFlag, int TEnumMax, typename TStorage> 
zFlag<TEnumFlag, TEnumMax, TStorage>::zFlag(bool _bInitAllActive) 
: muFlags(0)
{
	zenStaticAssertMsg( sizeof(TStorage)*8>=TEnumMax, "StorageContainerTooSmallForMaxValue" );
	if( _bInitAllActive )
		Invert();
};
*/
template<typename TEnumFlag, int TEnumMax, typename TStorage>
const zFlag<TEnumFlag, TEnumMax, TStorage>& zFlag<TEnumFlag, TEnumMax, TStorage>::operator= (const zFlag<TEnumFlag, TEnumMax, TStorage>& _Copy)		
{ 
	muFlags  = _Copy.muFlags;
	return *this;
}

template<typename TEnumFlag, int TEnumMax, typename TStorage>
const zFlag<TEnumFlag, TEnumMax, TStorage>& zFlag<TEnumFlag, TEnumMax, TStorage>::operator+=(const zFlag<TEnumFlag, TEnumMax, TStorage>& _Add)		
{ 
	muFlags |= _Add.muFlags;	
	return *this;
}

template<typename TEnumFlag, int TEnumMax, typename TStorage>
const zFlag<TEnumFlag, TEnumMax, TStorage>& zFlag<TEnumFlag, TEnumMax, TStorage>::operator-=(const zFlag<TEnumFlag, TEnumMax, TStorage>& _Rem)		
{ 
	muFlags &= ~_Rem.muFlags; 
	return *this;
}

template<typename TEnumFlag, int TEnumMax, typename TStorage>
const zFlag<TEnumFlag, TEnumMax, TStorage>& zFlag<TEnumFlag, TEnumMax, TStorage>::operator&=(const zFlag<TEnumFlag, TEnumMax, TStorage>& _And)		
{ 
	muFlags &= _And.muFlags;	
	return *this;
}

template<typename TEnumFlag, int TEnumMax, typename TStorage>
const zFlag<TEnumFlag, TEnumMax, TStorage>& zFlag<TEnumFlag, TEnumMax, TStorage>::operator|=(const zFlag<TEnumFlag, TEnumMax, TStorage>& _Or)		
{ 
	muFlags |= _Or.muFlags;	
	return *this;
}

template<typename TEnumFlag, int TEnumMax, typename TStorage>
const zFlag<TEnumFlag, TEnumMax, TStorage>& zFlag<TEnumFlag, TEnumMax, TStorage>::operator^=(const zFlag<TEnumFlag, TEnumMax, TStorage>& _Xor)		
{ 
	muFlags ^= _Xor.muFlags;	
	return *this;
}

template<typename TEnumFlag, int TEnumMax, typename TStorage>
zFlag<TEnumFlag, TEnumMax, TStorage> zFlag<TEnumFlag, TEnumMax, TStorage>::operator+ (const zFlag<TEnumFlag, TEnumMax, TStorage>& _Add)const
{
	zFlag Copy = *this;
	Copy += _Add;
	return Copy;
}

template<typename TEnumFlag, int TEnumMax, typename TStorage>
zFlag<TEnumFlag, TEnumMax, TStorage> zFlag<TEnumFlag, TEnumMax, TStorage>::operator- (const zFlag<TEnumFlag, TEnumMax, TStorage>& _Rem)const
{
	zFlag Copy = *this;
	Copy -= _Rem;
	return Copy;
}

template<typename TEnumFlag, int TEnumMax, typename TStorage>
zFlag<TEnumFlag, TEnumMax, TStorage> zFlag<TEnumFlag, TEnumMax, TStorage>::operator&(const zFlag<TEnumFlag, TEnumMax, TStorage>& _CmpFlag)const
{ 
	zFlag Flag;
	Flag.muFlags = muFlags & _CmpFlag.muFlags;
	return Flag;
}

template<typename TEnumFlag, int TEnumMax, typename TStorage>
zFlag<TEnumFlag, TEnumMax, TStorage> zFlag<TEnumFlag, TEnumMax, TStorage>::operator|(const zFlag<TEnumFlag, TEnumMax, TStorage>& _CmpFlag)const
{ 
	zFlag Flag;
	Flag.muFlags = muFlags | _CmpFlag.muFlags;
	return Flag;
}

template<typename TEnumFlag, int TEnumMax, typename TStorage>
zFlag<TEnumFlag, TEnumMax, TStorage> zFlag<TEnumFlag, TEnumMax, TStorage>::operator^(const zFlag<TEnumFlag, TEnumMax, TStorage>& _CmpFlag)const
{ 
	zFlag Flag;
	Flag.muFlags = muFlags ^ _CmpFlag.muFlags;
	return Flag;
}

template<typename TEnumFlag, int TEnumMax, typename TStorage>
const zFlag<TEnumFlag, TEnumMax, TStorage>& zFlag<TEnumFlag, TEnumMax, TStorage>::Reset()							
{ 
	muFlags = 0;				
	return *this;
}

template<typename TEnumFlag, int TEnumMax, typename TStorage>
const zFlag<TEnumFlag, TEnumMax, TStorage>& zFlag<TEnumFlag, TEnumMax, TStorage>::Invert()						
{ 
	muFlags = ~muFlags & ((TStorage(1)<<TEnumMax)-1); 
	return *this; 
}

template<typename TEnumFlag, int TEnumMax, typename TStorage>
bool zFlag<TEnumFlag, TEnumMax, TStorage>::Any( )const						
{ 
	return muFlags != 0; 
}

template<typename TEnumFlag, int TEnumMax, typename TStorage>
bool zFlag<TEnumFlag, TEnumMax, TStorage>::Any( const zFlag<TEnumFlag, TEnumMax, TStorage>& _Cmp )const	
{ 
	return (muFlags & (_Cmp.muFlags)) != 0;
}

template<typename TEnumFlag, int TEnumMax, typename TStorage>
bool zFlag<TEnumFlag, TEnumMax, TStorage>::All( const zFlag<TEnumFlag, TEnumMax, TStorage>& _Cmp )const	
{ 
	return (muFlags & (_Cmp.muFlags)) == _Cmp.muFlags;
}
	
template<typename TEnumFlag, int TEnumMax, typename TStorage>
bool zFlag<TEnumFlag, TEnumMax, TStorage>::operator==( const zFlag<TEnumFlag, TEnumMax, TStorage>& _Cmp )const
{ 
	return muFlags == _Cmp.muFlags;
}

template<typename TEnumFlag, int TEnumMax, typename TStorage>
bool zFlag<TEnumFlag, TEnumMax, TStorage>::operator!=( const zFlag<TEnumFlag, TEnumMax, TStorage>& _Cmp )const
{ 
	return muFlags != _Cmp.muFlags;
}

template<typename TEnumFlag, int TEnumMax, typename TStorage>
zU8 zFlag<TEnumFlag, TEnumMax, TStorage>::CountActive()
{
	return zenMath::BitsCount(muFlags);
}

template<typename TEnumFlag, int TEnumMax, typename TStorage>
zU8 zFlag<TEnumFlag, TEnumMax, TStorage>::CountInactive()
{
	return (TEnumMax-1) - zenMath::BitsCount(muFlags);
}

template<typename TEnumFlag, int TEnumMax, typename TStorage>
template<typename... Ts>
zFlag<TEnumFlag, TEnumMax, TStorage>::zFlag(TEnumFlag inFlag, Ts... r)
{
	muFlags = Mask(inFlag, r...);
}

template<typename TEnumFlag, int TEnumMax, typename TStorage>
template<typename... Ts>
bool zFlag<TEnumFlag, TEnumMax, TStorage>::Any(TEnumFlag inFlag, Ts... r)const
{
	TStorage uFlagCmp = Mask(inFlag, r...);
	return (muFlags & uFlagCmp) != 0;
}

template<typename TEnumFlag, int TEnumMax, typename TStorage>
template<typename... Ts>
bool zFlag<TEnumFlag, TEnumMax, TStorage>::All(TEnumFlag inFlag, Ts... r)const
{
	TStorage uFlagCmp = Mask(inFlag, r...);
	return (muFlags & uFlagCmp) == uFlagCmp;
}

template<typename TEnumFlag, int TEnumMax, typename TStorage>
template<typename... Ts>
constexpr TStorage zFlag<TEnumFlag, TEnumMax, TStorage>::Mask(TEnumFlag inFlag, Ts... r)
{
	zenAssertMsg((inFlag & (~((TStorage(1) << TEnumMax) - 1))) == 0, "Using an enum value higher than allowed");
	return (TStorage(1) << inFlag) | Mask(r...);
}




} } //namespace zen, Type
