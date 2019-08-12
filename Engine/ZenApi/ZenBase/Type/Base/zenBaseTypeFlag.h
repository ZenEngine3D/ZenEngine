#pragma once

namespace zen { namespace zenType {

template<typename TEnumFlag, int TEnumMax, typename TStorage=zU32>
class zFlag
{
public:
	zenInline						zFlag();
	//zenInline	explicit			zFlag(bool _bInitAllActive);
	template<typename... Ts>		zFlag(TEnumFlag inFlag, Ts... r);
	
	zenInline bool					Any()const;
	zenInline bool					Any(const zFlag& _Cmp)const;
	template<typename... Ts> bool	Any(TEnumFlag inFlag, Ts... r)const;

	zenInline bool					All(const zFlag& _Cmp)const;
	template<typename... Ts> bool	All(TEnumFlag inFlag, Ts... r)const;

	zenInline const zFlag&			operator= (const zFlag& _Cpy);
	zenInline const zFlag&			operator+=(const zFlag& _Add);
	zenInline const zFlag&			operator-=(const zFlag& _Rem);
	zenInline const zFlag&			operator&=(const zFlag& _And);
	zenInline const zFlag&			operator|=(const zFlag& _Or);
	zenInline const zFlag&			operator^=(const zFlag& _Xor);

	zenInline zFlag					operator+ (const zFlag& _Add)const;
	zenInline zFlag					operator- (const zFlag& _Rem)const;
	zenInline zFlag					operator&(const zFlag& _And)const;
	zenInline zFlag					operator|(const zFlag& _Or)const;
	zenInline zFlag					operator^(const zFlag& _Xor)const;
	
	zenInline const zFlag&			Reset();
	zenInline const zFlag&			Invert();
	zenInline zU8					CountActive();
	zenInline zU8					CountInactive();

	zenInline bool					operator==( const zFlag& _Cmp )const;
	zenInline bool					operator!=( const zFlag& _Cmp )const;
	
	template<typename... Ts>	
	constexpr static TStorage Mask(TEnumFlag Value, Ts... r);
	constexpr static TStorage Mask(){return 0;}
protected:		
	TStorage						muFlags;
};

template<typename TOut, typename TEnumType>
constexpr TOut zEnumMask(TEnumType _eEnumVal)
{
	//zenAssert( sizeof(TOut)*8 >= _eEnumVal ); //Not enough bits to convert enum to mask	
	return TOut(1) << _eEnumVal;
}

template<typename TOut, typename TEnumType, typename... Ts>
constexpr TOut zEnumMask(TEnumType _eEnumVal, Ts... _eRemainVals)
{
	return zEnumMask<TOut>(_eEnumVal) | zEnumMask<TOut>( _eRemainVals... );
}



} } //namespace zen, Type

#include "zenBaseTypeFlag.inl"
