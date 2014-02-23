#pragma once
#ifndef __LibCore_CMathRegion_h__
#define __LibCore_CMathRegion_h__

namespace zen { namespace awtype {

template<class _Type_>
class awRegion
{
AWClassDeclareNoParent(awRegion)
public:
	AWForceInline awRegion(void); //need to specialize for different types (right now there is a default implementation for awS8, awU8, awS16, awU16, awS32, awU32, awS64, awU64, awF32, awF64)
	AWForceInline awRegion(const _Type_ inX1, const _Type_ inY1, const _Type_ inX2, const _Type_ inY2);
	AWForceInline awRegion(const awRegion& xOther);

	AWForceInline bool operator!= (const awRegion& xOther);

	AWForceInline void Intersect(const awRegion& xBox1, const awRegion& xBox2);
	AWForceInline void Intersect(const awRegion& xBox);
	AWForceInline bool IsInside(_Type_ inX, _Type_ inY);

	AWForceInline void Offset(const awVector2<_Type_>* pxOffset);
	AWForceInline bool IsInside(const awVector2<_Type_>* pxOffset);

	AWForceInline bool IsNull(void);
	AWForceInline const awRegion& SetNull(void);

	union
	{
		struct {_Type_ mX1; _Type_ mY1; _Type_ mX2; _Type_ mY2; };
		struct {_Type_   x; _Type_   y; _Type_   z; _Type_   w; };
		struct {_Type_   r; _Type_   g; _Type_   b; _Type_   a; };
		_Type_ array[4];
	};
};

typedef awRegion<awS8> 	awRegionS8;		AWAddSupportsMemCopy(awRegionS8);
typedef awRegion<awU8> 	awRegionU8;		AWAddSupportsMemCopy(awRegionU8);
typedef awRegion<awS16>	awRegionS16;	AWAddSupportsMemCopy(awRegionS16);
typedef awRegion<awU16>	awRegionU16;	AWAddSupportsMemCopy(awRegionU16);
typedef awRegion<awS32>	awRegionS32;	AWAddSupportsMemCopy(awRegionS32);
typedef awRegion<awU32>	awRegionU32;	AWAddSupportsMemCopy(awRegionU32);
typedef awRegion<awS64>	awRegionS64;	AWAddSupportsMemCopy(awRegionS64);
typedef awRegion<awU64>	awRegionU64;	AWAddSupportsMemCopy(awRegionU64);
typedef awRegion<awF32>	awRegionF32;	AWAddSupportsMemCopy(awRegionF32);
typedef awRegion<awF64>	awRegionF64;	AWAddSupportsMemCopy(awRegionF64);

} }//namespace zen { namespace awtype {

#include "zenTypeRegion.inl"


#endif //__LibCore_CMathRegion_h__
