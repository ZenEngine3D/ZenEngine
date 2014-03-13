#include "libZenBase.h"

namespace zen { namespace zenType {

zUInt GrowthPolicyOneandHalf(zUInt _uCurrentCount, zUInt _uNeededCount, zUInt _uItemSize)
{
	#define RoundUpValue 128
	zUInt uItemCount = (_uNeededCount * 15) / 10;								// Grow item count by 1.5
	zUInt uSize		= uItemCount * _uItemSize;		
	uSize			= ((uSize + RoundUpValue-1) / RoundUpValue) * RoundUpValue;	// Round up memory used by 128 Bytes
	return uSize / _uItemSize;													// Return number of item to allocate
}

} } //namespace zen, Type
