#pragma once
#ifndef __LibCore_Api_Type_Rect_h__
#define __LibCore_Api_Type_Rect_h__

namespace zen { namespace awtype {

template<class _Type_>
class awRect
{
AWClassDeclareNoParent(awRect)
public:
	AWForceInline awRect(void); //need to specialize for different types (right now there is a default implementation for awS8, awU8, awS16, awU16, awS32, awU32, awS64, awU64, awF32, awF64)
	AWForceInline awRect(const _Type_& inX, const _Type_& inY, const _Type_& inW, const _Type_& inH);
	AWForceInline awRect(const awRect& xOther);

	AWForceInline bool IsNull(void);
	AWForceInline const awRect& SetNull(void);

	union
	{
		struct {_Type_ mX; _Type_ mY; _Type_ mW; _Type_ mH; };
		struct {_Type_  x; _Type_  y; _Type_  z; _Type_  w; };
		struct {_Type_  r; _Type_  g; _Type_  b; _Type_  a; };
		_Type_ array[4];
	};
};

typedef awRect<awS8> 	awRectS8;	AWAddSupportsMemCopy(awRectS8);
typedef awRect<awU8> 	awRectU8;	AWAddSupportsMemCopy(awRectU8);
typedef awRect<awS16>	awRectS16;	AWAddSupportsMemCopy(awRectS16);
typedef awRect<awU16>	awRectU16;	AWAddSupportsMemCopy(awRectU16);
typedef awRect<awS32>	awRectS32;	AWAddSupportsMemCopy(awRectS32);
typedef awRect<awU32>	awRectU32;	AWAddSupportsMemCopy(awRectU32);
typedef awRect<awS64>	awRectS64;	AWAddSupportsMemCopy(awRectS64);
typedef awRect<awU64>	awRectU64;	AWAddSupportsMemCopy(awRectU64);
typedef awRect<awF32>	awRectF32;	AWAddSupportsMemCopy(awRectF32);
typedef awRect<awF64>	awRectF64;	AWAddSupportsMemCopy(awRectF64);

} }//namespace zen { namespace awtype {

#include "zenTypeRect.inl"

#endif //__LibCore_Api_Type_Rect_h__
