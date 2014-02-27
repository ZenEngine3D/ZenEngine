#pragma once
#ifndef __zenBase_Type_Rect_h__
#define __zenBase_Type_Rect_h__

namespace zen { namespace zenType {

template<class _Type_>
class zenRect
{
ZENClassDeclareNoParent(zenRect)
public:
	AWForceInline zenRect(void); //need to specialize for different types (right now there is a default implementation for zenS8, zenU8, zenS16, zenU16, zenS32, zenU32, zenS64, zenU64, zenF32, zenF64)
	AWForceInline zenRect(const _Type_& inX, const _Type_& inY, const _Type_& inW, const _Type_& inH);
	AWForceInline zenRect(const zenRect& xOther);

	AWForceInline bool IsNull(void);
	AWForceInline const zenRect& SetNull(void);

	union
	{
		struct {_Type_ mX; _Type_ mY; _Type_ mW; _Type_ mH; };
		struct {_Type_  x; _Type_  y; _Type_  z; _Type_  w; };
		struct {_Type_  r; _Type_  g; _Type_  b; _Type_  a; };
		_Type_ array[4];
	};
};

typedef zenRect<zenS8> 	zenRectS8;	ZENSupportMemcopy(zenRectS8);
typedef zenRect<zenU8> 	zenRectU8;	ZENSupportMemcopy(zenRectU8);
typedef zenRect<zenS16>	zenRectS16;	ZENSupportMemcopy(zenRectS16);
typedef zenRect<zenU16>	zenRectU16;	ZENSupportMemcopy(zenRectU16);
typedef zenRect<zenS32>	zenRectS32;	ZENSupportMemcopy(zenRectS32);
typedef zenRect<zenU32>	zenRectU32;	ZENSupportMemcopy(zenRectU32);
typedef zenRect<zenS64>	zenRectS64;	ZENSupportMemcopy(zenRectS64);
typedef zenRect<zenU64>	zenRectU64;	ZENSupportMemcopy(zenRectU64);
typedef zenRect<zenF32>	zenRectF32;	ZENSupportMemcopy(zenRectF32);
typedef zenRect<zenF64>	zenRectF64;	ZENSupportMemcopy(zenRectF64);

} }//namespace zen { namespace zenType {

#include "zenTypeRect.inl"

#endif //__zenBase_Type_Rect_h__
