#include "zbBase.h"

namespace zen { namespace zenMath 
{
	const zU32 uFFFFFFFF = 0xFFFFFFFF;
	const zU32 u00000000 = 0;
    const V4 v4Zero		= simdVector(0.0f);
    const V4 v4One      = simdVector(1.0f);
	const V4 v4OneNeg	= simdVector(-1.f);
	const V4 v4X1		= simdXYZW(1.0f, 0.0f, 0.0f, 0.0f);
    const V4 v4Y1		= simdXYZW(0.0f, 1.0f, 0.0f, 0.0f);  
    const V4 v4Z1		= simdXYZW(0.0f, 0.0f, 1.0f, 0.0f);
    const V4 v4W1		= simdXYZW(0.0f, 0.0f, 0.0f, 1.0f);
	const V4 v4XYZ1		= simdXYZW(1.0f, 1.0f, 1.0f, 0.0f);
	const V4 v4Mask3    = simdXYZW(*(float*)&uFFFFFFFF, *(float*)&uFFFFFFFF, *(float*)&uFFFFFFFF, *(float*)&u00000000);
	const V4 v4MaskW    = simdXYZW(*(float*)&u00000000, *(float*)&u00000000, *(float*)&u00000000, *(float*)&uFFFFFFFF);

}  } // namespace zen, zenMath
