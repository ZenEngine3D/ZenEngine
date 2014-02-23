#include "libZenBase.h"

namespace zen { namespace zenMath 
{
	const awU32 uFFFFFFFF = 0xFFFFFFFF;
	const awU32 u00000000 = 0;
    const V4 v4Zero		= Vector(0.0f);
    const V4 v4One      = Vector(1.0f);
	const V4 v4OneNeg	= Vector(-1.f);
	const V4 v4X1		= XYZW(1.0f, 0.0f, 0.0f, 0.0f);
    const V4 v4Y1		= XYZW(0.0f, 1.0f, 0.0f, 0.0f);  
    const V4 v4Z1		= XYZW(0.0f, 0.0f, 1.0f, 0.0f);
    const V4 v4W1		= XYZW(0.0f, 0.0f, 0.0f, 1.0f);
	const V4 v4XYZ1		= XYZW(1.0f, 1.0f, 1.0f, 0.0f);
	const V4 v4Mask3    = XYZW(*(float*)&uFFFFFFFF, *(float*)&uFFFFFFFF, *(float*)&uFFFFFFFF, *(float*)&u00000000);
	const V4 v4MaskW    = XYZW(*(float*)&u00000000, *(float*)&u00000000, *(float*)&u00000000, *(float*)&uFFFFFFFF);

}  } // namespace zen, zenMath
