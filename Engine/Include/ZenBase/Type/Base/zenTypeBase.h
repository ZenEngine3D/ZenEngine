#pragma once
#ifndef __LibCore_Api_Type_Base_h__
#define __LibCore_Api_Type_Base_h__

#include <stdint.h>

namespace zen { namespace awtype {

typedef int8_t			awS8;		//!< Basic integer type
typedef int16_t			awS16;		//!< Basic integer type
typedef int32_t			awS32;		//!< Basic integer type
typedef int64_t			awS64;		//!< Basic integer type
typedef uint8_t			awU8;		//!< Basic integer type
typedef uint16_t		awU16;		//!< Basic integer type
typedef uint32_t		awU32;		//!< Basic integer type
typedef uint64_t		awU64;		//!< Basic integer type
typedef float			awF32;		//!< Basic integer type
typedef double			awF64;		//!< Basic integer type
typedef int				awInt;		//!< Basic int
typedef unsigned int	awUInt;		//!< Basic unsigned int
typedef uintptr_t		awPointer;	

AWAddSupportsMemCopy(awS8);
AWAddSupportsMemCopy(awS16);
AWAddSupportsMemCopy(awS32);
AWAddSupportsMemCopy(awS64);
AWAddSupportsMemCopy(awU8);
AWAddSupportsMemCopy(awU16);
AWAddSupportsMemCopy(awU32);
AWAddSupportsMemCopy(awU64);
AWAddSupportsMemCopy(awF32);
AWAddSupportsMemCopy(awF64);

}} //namespace zen, awtype

#endif
