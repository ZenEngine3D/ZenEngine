#pragma once
#ifndef __zenApi_Base_Type_Base_h__
#define __zenApi_Base_Type_Base_h__

#include <stdint.h>
#include <array>
#include <bitset>

namespace zen { namespace zenType {

typedef int8_t			zI8;		//!< Basic integer type
typedef int16_t			zI16;		//!< Basic integer type
typedef int32_t			zI32;		//!< Basic integer type
typedef int64_t			zI64;		//!< Basic integer type
typedef uint8_t			zU8;		//!< Basic unsigned integer type
typedef uint16_t		zU16;		//!< Basic unsigned integer type
typedef uint32_t		zU32;		//!< Basic unsigned integer type
typedef uint64_t		zU64;		//!< Basic unsigned integer type

#if !_WIN64 //! @todo Clean find better way to determine register size
typedef int32_t			zInt;		//!< Basic unsigned integer type
typedef uint32_t		zUInt;		//!< Basic unsigned integer type
#else
typedef int64_t			zInt;		//!< Basic unsigned integer type
typedef uint64_t		zUInt;		//!< Basic unsigned integer type
#endif

//! @todo Cleanup : Find better location for this utility method
template<class TClass, class TMemberClass>
zU32 zenOffsetOf( TMemberClass TClass::* _pMember)
{
	union GetOffset
	{
		GetOffset(typename TMemberClass TClass::* _Member)
		: mpMember(_Member)
		{
		}
		zU32 mOffset;
		typename TMemberClass TClass::* mpMember;
	};
	return GetOffset( _pMember ).mOffset;
};

template<typename T> void zenSwap(T& _Val1, T& _Val2)
{
	T Temp(_Val2);
	_Val2 = _Val1;
	_Val1 = Temp;
}

}} //namespace zen, zenType

#endif
