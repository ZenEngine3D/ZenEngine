#pragma once

namespace zen { namespace zenType 
{

template<class _Type_>
class zRegion
{
zenClassDeclareNoParent(zRegion)
public:
	zenInlineForce zRegion(void); //need to specialize for different types (right now there is a default implementation for zI8, zU8, zI16, zU16, zI32, zU32, zI64, zU64, float, double)
	zenInlineForce zRegion(const _Type_ inX1, const _Type_ inY1, const _Type_ inX2, const _Type_ inY2);
	zenInlineForce zRegion(const zRegion& xOther);

	zenInlineForce bool operator!= (const zRegion& xOther);

	zenInlineForce void Intersect(const zRegion& xBox1, const zRegion& xBox2);
	zenInlineForce void Intersect(const zRegion& xBox);
	zenInlineForce bool IsInside(_Type_ inX, _Type_ inY);

	zenInlineForce void Offset(const zVector2<_Type_>* pxOffset);
	zenInlineForce bool IsInside(const zVector2<_Type_>* pxOffset);

	zenInlineForce bool IsZero(void);
	zenInlineForce const zRegion& SetZero(void);

	union
	{
		struct {_Type_ mX1; _Type_ mY1; _Type_ mX2; _Type_ mY2; };
		struct {_Type_   x; _Type_   y; _Type_   z; _Type_   w; };
		struct {_Type_   r; _Type_   g; _Type_   b; _Type_   a; };
		_Type_ array[4];
	};
};

typedef zRegion<zI8> 	zRegionS8;	
typedef zRegion<zU8> 	zRegionU8;	
typedef zRegion<zI16>	zRegionS16;	
typedef zRegion<zU16>	zRegionU16;	
typedef zRegion<zI32>	zRegionS32;	
typedef zRegion<zU32>	zRegionU32;	
typedef zRegion<zI64>	zRegionS64;	
typedef zRegion<zU64>	zRegionU64;	
typedef zRegion<float>	zRegionF32;	
typedef zRegion<double>	zRegionF64;	

} }//namespace zen { namespace zenType {

#include "zenBaseTypeRegion.inl"

