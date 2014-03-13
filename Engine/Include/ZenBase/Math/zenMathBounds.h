#pragma once
#ifndef __LibZenBase_Api_Math_Bounds_h__
#define __LibZenBase_Api_Math_Bounds_h__

namespace zen { namespace zenMath 
{
	//! @todo implement this
	class Bounds
	{
	public:
		Bounds()
		: mvCenterRadius(0,0,0,999999)
		, mvBoxMin(-999999,-999999,-999999)
		, mvBoxMax(999999,999999,999999)
		{
		}

		Bounds( const zVec3F& _vCenter, float _fRadius )
		: mvCenterRadius(_vCenter.x, _vCenter.y, _vCenter.z, _fRadius)
		{
		}

		Bounds( const zVec3F& _vBoxMin, const zVec3F& _vBoxMax )
		: mvBoxMin(_vBoxMin)
		, mvBoxMax(_vBoxMax)
		{
			zVec3F vDiff;
			//mvBoxMin + (mvBoxMax - mvBoxMin) / 2.f;
			//zVec3F vCenter = mvBoxMin + (mvBoxMax - mvBoxMin) / 2.f;
			//mvCenterRadius
		}

	protected:
		zVec4F	mvCenterRadius;	//!< Center and radius of sphere bounds
		zVec3F	mvBoxMin;		//!< Minimum coordinates of box bounds
		zVec3F	mvBoxMax;		//!< Maximum coordinates of box bounds
	};
}  } // namespace zen, zenMath

#endif