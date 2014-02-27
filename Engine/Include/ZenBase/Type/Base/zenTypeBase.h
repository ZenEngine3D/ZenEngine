#pragma once
#ifndef __zenBase_Type_Base_h__
#define __zenBase_Type_Base_h__

#include <stdint.h>

namespace zen { namespace zenType {

typedef int8_t			zenS8;		//!< Basic integer type
typedef int16_t			zenS16;		//!< Basic integer type
typedef int32_t			zenS32;		//!< Basic integer type
typedef int64_t			zenS64;		//!< Basic integer type
typedef uint8_t			zenU8;		//!< Basic integer type
typedef uint16_t		zenU16;		//!< Basic integer type
typedef uint32_t		zenU32;		//!< Basic integer type
typedef uint64_t		zenU64;		//!< Basic integer type
typedef float			zenF32;		//!< Basic integer type
typedef double			zenF64;		//!< Basic integer type
typedef int				zenInt;		//!< Basic int
typedef unsigned int	zenUInt;	//!< Basic unsigned int
typedef uintptr_t		zenPointer;	

ZENSupportMemcopy(zenS8);
ZENSupportMemcopy(zenS16);
ZENSupportMemcopy(zenS32);
ZENSupportMemcopy(zenS64);
ZENSupportMemcopy(zenU8);
ZENSupportMemcopy(zenU16);
ZENSupportMemcopy(zenU32);
ZENSupportMemcopy(zenU64);
ZENSupportMemcopy(zenF32);
ZENSupportMemcopy(zenF64);

}} //namespace zen, zenType

#endif
