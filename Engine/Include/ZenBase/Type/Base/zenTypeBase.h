#pragma once
#ifndef __zenBase_Type_Base_h__
#define __zenBase_Type_Base_h__

#include <stdint.h>

namespace zen { namespace zenType {

typedef int8_t			zI8;		//!< Basic integer type
typedef int16_t			zI16;		//!< Basic integer type
typedef int32_t			zI32;		//!< Basic integer type
typedef int64_t			zI64;		//!< Basic integer type
typedef uint8_t			zU8;		//!< Basic integer type
typedef uint16_t		zU16;		//!< Basic integer type
typedef uint32_t		zU32;		//!< Basic integer type
typedef uint64_t		zU64;		//!< Basic integer type
typedef int				zInt;		//!< Basic int
typedef unsigned int	zUInt;		//!< Basic unsigned int


ZENSupportMemcopy(zI8);
ZENSupportMemcopy(zI16);
ZENSupportMemcopy(zI32);
ZENSupportMemcopy(zI64);
ZENSupportMemcopy(zU8);
ZENSupportMemcopy(zU16);
ZENSupportMemcopy(zU32);
ZENSupportMemcopy(zU64);
ZENSupportMemcopy(float);
ZENSupportMemcopy(double);

}} //namespace zen, zenType

#endif
