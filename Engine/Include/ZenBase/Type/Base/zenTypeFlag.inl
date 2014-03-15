namespace zen { namespace zenType {

template<typename TEnumFlag, int TEnumMax, typename TStorage> 
zFlag<TEnumFlag, TEnumMax, TStorage>::zFlag() 
: muFlags(0)
{
	ZENStaticAssertMsg( sizeof(TStorage)*8>=TEnumMax, StorageContainerTooSmallForMaxValue );
};

template<typename TEnumFlag, int TEnumMax, typename TStorage> 
zFlag<TEnumFlag, TEnumMax, TStorage>::zFlag(TStorage _Mask) 
: muFlags(_Mask)
{
};

template<typename TEnumFlag, int TEnumMax, typename TStorage>
const zFlag<TEnumFlag, TEnumMax, TStorage>& zFlag<TEnumFlag, TEnumMax, TStorage>::operator= (const zFlag<TEnumFlag, TEnumMax, TStorage>& _Cpy)		
{ 
	muFlags  = _Cpy.muFlags;	
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
zFlag<TEnumFlag, TEnumMax, TStorage> zFlag<TEnumFlag, TEnumMax, TStorage>::operator&(const zFlag<TEnumFlag, TEnumMax, TStorage>& _And)		
{ 
	return zFlag(muFlags & _And.muFlags);
}

template<typename TEnumFlag, int TEnumMax, typename TStorage>
zFlag<TEnumFlag, TEnumMax, TStorage> zFlag<TEnumFlag, TEnumMax, TStorage>::operator|(const zFlag<TEnumFlag, TEnumMax, TStorage>& _And)		
{ 
	return zFlag(muFlags | _And.muFlags);
}

template<typename TEnumFlag, int TEnumMax, typename TStorage>
zFlag<TEnumFlag, TEnumMax, TStorage> zFlag<TEnumFlag, TEnumMax, TStorage>::operator^(const zFlag<TEnumFlag, TEnumMax, TStorage>& _And)		
{ 
	return zFlag(muFlags ^ _And.muFlags);
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
zUInt zFlag<TEnumFlag, TEnumMax, TStorage>::CountActive()
{
	return zenMath::BitsCount(muFlags);
}

template<typename TEnumFlag, int TEnumMax, typename TStorage>
zUInt zFlag<TEnumFlag, TEnumMax, TStorage>::CountInactive()
{
	return (TEnumMax-1) - zenMath::BitsCount(muFlags);
}

/*
template<typename TEnumFlag, int TEnumMax, typename TStorage>
const zFlag<TEnumFlag, TEnumMax, TStorage>& zFlag<TEnumFlag, TEnumMax, TStorage>::ForceSet(TStorage _FlagValues)
{
	CheckValid(_FlagValues);
	muFlags = _FlagValues;
	return *this;
}
*/

template<typename TEnumFlag, int TEnumMax, typename TStorage>
AWForceInline void zFlag<TEnumFlag, TEnumMax, TStorage>::CheckValid(TStorage _Value)
{
	ZENAssertMsg( (_Value & (~((TStorage(1)<<TEnumMax)-1)))==0, "Using an enum value higher than allowed");
}

template<typename TEnumFlag, int TEnumMax, typename TStorage>
bool zFlag<TEnumFlag, TEnumMax, TStorage>::Any( TStorage _uFlags )
{
	CheckValid(_uFlags);		
	return (muFlags & _uFlags) != 0;
}

template<typename TEnumFlag, int TEnumMax, typename TStorage>
bool zFlag<TEnumFlag, TEnumMax, TStorage>::All( TStorage _uFlags )
{
	CheckValid(_uFlags);		
	return (muFlags & _uFlags) == _uFlags;
}

} } //namespace zen, Type
