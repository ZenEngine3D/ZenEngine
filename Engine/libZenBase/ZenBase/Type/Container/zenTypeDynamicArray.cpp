#include "libZenBase.h"

namespace zen { namespace zenType {

zenUInt GrowthPolicyOneandHalf(zenUInt _uCurrentCount, zenUInt _uNeededCount, zenUInt _uItemSize)
{
	#define RoundUpValue 128
	zenUInt uItemCount = (_uNeededCount * 15) / 10;								// Grow item count by 1.5
	zenUInt uSize		= uItemCount * _uItemSize;		
	uSize			= ((uSize + RoundUpValue-1) / RoundUpValue) * RoundUpValue;	// Round up memory used by 128 Bytes
	return uSize / _uItemSize;													// Return number of item to allocate
}

} } //namespace zen, Type
