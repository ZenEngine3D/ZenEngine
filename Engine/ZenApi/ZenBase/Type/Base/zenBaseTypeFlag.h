#pragma once
#ifndef __zenApi_Base_Type_Flag_h__
#define __zenApi_Base_Type_Flag_h__

namespace zen { namespace zenType {

template<typename TEnumFlag, int TEnumMax, typename TStorage=zU32>
class zFlag
{
public:
	ZENInline						zFlag();
	ZENInline						zFlag(bool _bInitAllActive);
	template<typename... Ts>		zFlag(Ts... r);
	
	ZENInline bool					Any()const;
	ZENInline bool					Any(const zFlag& _Cmp)const;
	template<typename... Ts> bool	Any(Ts... r)const;

	ZENInline bool					All(const zFlag& _Cmp)const;
	template<typename... Ts> bool	All(Ts... r)const;

	ZENInline const zFlag&			operator= (const zFlag& _Cpy);
	ZENInline const zFlag&			operator+=(const zFlag& _Add);
	ZENInline const zFlag&			operator-=(const zFlag& _Rem);
	ZENInline const zFlag&			operator&=(const zFlag& _And);
	ZENInline const zFlag&			operator|=(const zFlag& _Or);
	ZENInline const zFlag&			operator^=(const zFlag& _Xor);

	ZENInline zFlag					operator+ (const zFlag& _Add)const;
	ZENInline zFlag					operator- (const zFlag& _Rem)const;
	ZENInline zFlag					operator&(const zFlag& _And)const;
	ZENInline zFlag					operator|(const zFlag& _Or)const;
	ZENInline zFlag					operator^(const zFlag& _Xor)const;
	
	ZENInline const zFlag&			Reset();
	ZENInline const zFlag&			Invert();
	ZENInline zU8					CountActive();
	ZENInline zU8					CountInactive();

	ZENInline bool					operator==( const zFlag& _Cmp )const;
	ZENInline bool					operator!=( const zFlag& _Cmp )const;
	
	template<typename... Ts>
	constexpr static TStorage		Mask(TEnumFlag Value, Ts... r);
	constexpr static TStorage		Mask(TEnumFlag _Value);
protected:		
	ZENInline						zFlag(TStorage _Mask);
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

#endif
