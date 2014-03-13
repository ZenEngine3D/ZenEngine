#pragma once
#ifndef __LibZenBase_CMathRegion_h__
#define __LibZenBase_CMathRegion_h__

namespace zen { namespace zenType {

template<class _Type_>
class zRegion
{
ZENClassDeclareNoParent(zRegion)
public:
	AWForceInline zRegion(void); //need to specialize for different types (right now there is a default implementation for zS8, zU8, zS16, zU16, zS32, zU32, zS64, zU64, float, double)
	AWForceInline zRegion(const _Type_ inX1, const _Type_ inY1, const _Type_ inX2, const _Type_ inY2);
	AWForceInline zRegion(const zRegion& xOther);

	AWForceInline bool operator!= (const zRegion& xOther);

	AWForceInline void Intersect(const zRegion& xBox1, const zRegion& xBox2);
	AWForceInline void Intersect(const zRegion& xBox);
	AWForceInline bool IsInside(_Type_ inX, _Type_ inY);

	AWForceInline void Offset(const zVector2<_Type_>* pxOffset);
	AWForceInline bool IsInside(const zVector2<_Type_>* pxOffset);

	AWForceInline bool IsNull(void);
	AWForceInline const zRegion& SetNull(void);

	union
	{
		struct {_Type_ mX1; _Type_ mY1; _Type_ mX2; _Type_ mY2; };
		struct {_Type_   x; _Type_   y; _Type_   z; _Type_   w; };
		struct {_Type_   r; _Type_   g; _Type_   b; _Type_   a; };
		_Type_ array[4];
	};
};

typedef zRegion<zS8> 	zRegionS8;	ZENSupportMemcopy(zRegionS8);
typedef zRegion<zU8> 	zRegionU8;	ZENSupportMemcopy(zRegionU8);
typedef zRegion<zS16>	zRegionS16;	ZENSupportMemcopy(zRegionS16);
typedef zRegion<zU16>	zRegionU16;	ZENSupportMemcopy(zRegionU16);
typedef zRegion<zS32>	zRegionS32;	ZENSupportMemcopy(zRegionS32);
typedef zRegion<zU32>	zRegionU32;	ZENSupportMemcopy(zRegionU32);
typedef zRegion<zS64>	zRegionS64;	ZENSupportMemcopy(zRegionS64);
typedef zRegion<zU64>	zRegionU64;	ZENSupportMemcopy(zRegionU64);
typedef zRegion<float>	zRegionF32;	ZENSupportMemcopy(zRegionF32);
typedef zRegion<double>	zRegionF64;	ZENSupportMemcopy(zRegionF64);

} }//namespace zen { namespace zenType {

#include "zenTypeRegion.inl"


#endif //__LibZenBase_CMathRegion_h__
