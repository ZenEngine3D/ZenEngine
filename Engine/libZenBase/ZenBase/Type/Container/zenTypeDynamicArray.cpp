#include "libZenBase.h"

namespace zen { namespace awtype {

awUInt GrowthPolicyOneandHalf(awUInt _uCurrentCount, awUInt _uNeededCount, awUInt _uItemSize)
{
	#define RoundUpValue 128
	awUInt uItemCount = (_uNeededCount * 15) / 10;								// Grow item count by 1.5
	awUInt uSize		= uItemCount * _uItemSize;		
	uSize			= ((uSize + RoundUpValue-1) / RoundUpValue) * RoundUpValue;	// Round up memory used by 128 Bytes
	return uSize / _uItemSize;													// Return number of item to allocate
}

} } //namespace zen, Type
