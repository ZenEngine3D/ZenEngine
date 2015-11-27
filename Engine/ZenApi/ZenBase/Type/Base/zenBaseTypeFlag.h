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

protected:
	template<typename... Ts>
	constexpr static TStorage		MakeFlag(TEnumFlag Value, Ts... r);	//! Using recursive variadic template to handle any parameters count
	constexpr static TStorage		MakeFlag(TEnumFlag _Value);
	ZENInline						zFlag(TStorage _Mask);
	TStorage						muFlags;
};

} } //namespace zen, Type

#include "zenBaseTypeFlag.inl"

#endif
