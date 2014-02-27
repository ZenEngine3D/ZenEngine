#pragma once
#ifndef __LibZenBase_CMathRegion_h__
#define __LibZenBase_CMathRegion_h__

namespace zen { namespace zenType {

template<class _Type_>
class zenRegion
{
ZENClassDeclareNoParent(zenRegion)
public:
	AWForceInline zenRegion(void); //need to specialize for different types (right now there is a default implementation for zenS8, zenU8, zenS16, zenU16, zenS32, zenU32, zenS64, zenU64, zenF32, zenF64)
	AWForceInline zenRegion(const _Type_ inX1, const _Type_ inY1, const _Type_ inX2, const _Type_ inY2);
	AWForceInline zenRegion(const zenRegion& xOther);

	AWForceInline bool operator!= (const zenRegion& xOther);

	AWForceInline void Intersect(const zenRegion& xBox1, const zenRegion& xBox2);
	AWForceInline void Intersect(const zenRegion& xBox);
	AWForceInline bool IsInside(_Type_ inX, _Type_ inY);

	AWForceInline void Offset(const zenVector2<_Type_>* pxOffset);
	AWForceInline bool IsInside(const zenVector2<_Type_>* pxOffset);

	AWForceInline bool IsNull(void);
	AWForceInline const zenRegion& SetNull(void);

	union
	{
		struct {_Type_ mX1; _Type_ mY1; _Type_ mX2; _Type_ mY2; };
		struct {_Type_   x; _Type_   y; _Type_   z; _Type_   w; };
		struct {_Type_   r; _Type_   g; _Type_   b; _Type_   a; };
		_Type_ array[4];
	};
};

typedef zenRegion<zenS8> 	awRegionS8;		ZENSupportMemcopy(awRegionS8);
typedef zenRegion<zenU8> 	awRegionU8;		ZENSupportMemcopy(awRegionU8);
typedef zenRegion<zenS16>	awRegionS16;	ZENSupportMemcopy(awRegionS16);
typedef zenRegion<zenU16>	awRegionU16;	ZENSupportMemcopy(awRegionU16);
typedef zenRegion<zenS32>	awRegionS32;	ZENSupportMemcopy(awRegionS32);
typedef zenRegion<zenU32>	awRegionU32;	ZENSupportMemcopy(awRegionU32);
typedef zenRegion<zenS64>	awRegionS64;	ZENSupportMemcopy(awRegionS64);
typedef zenRegion<zenU64>	awRegionU64;	ZENSupportMemcopy(awRegionU64);
typedef zenRegion<zenF32>	awRegionF32;	ZENSupportMemcopy(awRegionF32);
typedef zenRegion<zenF64>	awRegionF64;	ZENSupportMemcopy(awRegionF64);

} }//namespace zen { namespace zenType {

#include "zenTypeRegion.inl"


#endif //__LibZenBase_CMathRegion_h__
