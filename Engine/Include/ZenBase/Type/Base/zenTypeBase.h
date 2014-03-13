#pragma once
#ifndef __zenBase_Type_Base_h__
#define __zenBase_Type_Base_h__

#include <stdint.h>

namespace zen { namespace zenType {

typedef int8_t			zS8;		//!< Basic integer type
typedef int16_t			zS16;		//!< Basic integer type
typedef int32_t			zS32;		//!< Basic integer type
typedef int64_t			zS64;		//!< Basic integer type
typedef uint8_t			zU8;		//!< Basic integer type
typedef uint16_t		zU16;		//!< Basic integer type
typedef uint32_t		zU32;		//!< Basic integer type
typedef uint64_t		zU64;		//!< Basic integer type
typedef int				zInt;		//!< Basic int
typedef unsigned int	zUInt;		//!< Basic unsigned int


ZENSupportMemcopy(zS8);
ZENSupportMemcopy(zS16);
ZENSupportMemcopy(zS32);
ZENSupportMemcopy(zS64);
ZENSupportMemcopy(zU8);
ZENSupportMemcopy(zU16);
ZENSupportMemcopy(zU32);
ZENSupportMemcopy(zU64);
ZENSupportMemcopy(float);
ZENSupportMemcopy(double);

}} //namespace zen, zenType

#endif
