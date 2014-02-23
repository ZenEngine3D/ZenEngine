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

AWAddSupportsMemCopy(zenS8);
AWAddSupportsMemCopy(zenS16);
AWAddSupportsMemCopy(zenS32);
AWAddSupportsMemCopy(zenS64);
AWAddSupportsMemCopy(zenU8);
AWAddSupportsMemCopy(zenU16);
AWAddSupportsMemCopy(zenU32);
AWAddSupportsMemCopy(zenU64);
AWAddSupportsMemCopy(zenF32);
AWAddSupportsMemCopy(zenF64);

}} //namespace zen, zenType

#endif
