#pragma once
#ifndef __LibCore_Api_Math_Bounds_h__
#define __LibCore_Api_Math_Bounds_h__

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

		Bounds( const awVec3F& _vCenter, float _fRadius )
		: mvCenterRadius(_vCenter.x, _vCenter.y, _vCenter.z, _fRadius)
		{
		}

		Bounds( const awVec3F& _vBoxMin, const awVec3F& _vBoxMax )
		: mvBoxMin(_vBoxMin)
		, mvBoxMax(_vBoxMax)
		{
			awVec3F vDiff;
			//mvBoxMin + (mvBoxMax - mvBoxMin) / 2.f;
			//awVec3F vCenter = mvBoxMin + (mvBoxMax - mvBoxMin) / 2.f;
			//mvCenterRadius
		}

	protected:
		awVec4F	mvCenterRadius;	//!< Center and radius of sphere bounds
		awVec3F	mvBoxMin;		//!< Minimum coordinates of box bounds
		awVec3F	mvBoxMax;		//!< Maximum coordinates of box bounds
	};
}  } // namespace zen, zenMath

#endif