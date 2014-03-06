#pragma once
#ifndef __LibZenBase_Api_Math_Vector4_h__
#define __LibZenBase_Api_Math_Vector4_h__

#include <xmmintrin.h>

namespace zen { namespace zenMath 
{
    typedef __m128 V4;

    // Values Set
    ZENInline V4		Point           ( float _fValue         );
    ZENInline V4		Point           ( const float* _pValues );
    ZENInline V4		Point           ( const float _fX, const float _fY, const float _fZ);
    ZENInline V4		Vector          ( float _fValue         );
    ZENInline V4		Vector          ( const float* _pValues );
    ZENInline V4		Vector          ( const float _fX, const float _fY, const float _fZ);

    ZENInline V4		XYZW            ( float _fValue         );
    ZENInline V4		XYZW            ( const float* _pValues );
    ZENInline V4		XYZW            ( float _fX, float _fY, float _fZ, float _fW);
	/*
	ZENInline V4		XYZW            ( zenU32 _uValue         );
	ZENInline V4		XYZW            ( const zenU32* _uValues );
	ZENInline V4		XYZW            ( zenU32 _uX, zenU32 _uY, zenU32 _uZ, zenU32 _uW);
	*/
    ZENInline V4		SpreadX         ( V4 _Vect );
    ZENInline V4		SpreadY         ( V4 _Vect );
    ZENInline V4		SpreadZ         ( V4 _Vect );
    ZENInline V4		SpreadW         ( V4 _Vect );    
  //  ZENInline V4  Shuffle         ( V4 _Vect, int _uMask);              //.xyzw = Values picked from _Vect.xyzw, moved around (use macro SHUFFLE_MASK)
  //  ZENInline V4  Shuffle         ( V4 _VectA, V4 _VectB, int _Mask);   //.xy = 2 values picked from _VectA.xyzw and .zw = 2 values from _VectB.xyzw (use macro SHUFFLE_MASK)

    // Values Get
    ZENInline void		XYZW            ( V4 _Vect, float* _pValuesOut );
    ZENInline float	X               ( V4 _Vect );
    ZENInline float	Y               ( V4 _Vect );
    ZENInline float	Z               ( V4 _Vect );
    ZENInline float	W               ( V4 _Vect );    
    
	// Arithmerics
    ZENInline V4		Add				( V4 _VectA, V4 _VectB );
    ZENInline V4		Sub				( V4 _VectA, V4 _VectB );
    ZENInline V4		Mul				( V4 _VectA, V4 _VectB );
    ZENInline V4		Div				( V4 _VectA, V4 _VectB );
    ZENInline V4		MAdd			( V4 _VectA, V4 _VectB, V4 _VectC);  //(A*B)+C
    ZENInline V4		Max				( V4 _VectA, V4 _VectB );
    ZENInline V4		Min				( V4 _VectA, V4 _VectB );    
    ZENInline V4		Cross			( V4 _VectA, V4 _VectB );
    ZENInline V4		DotV			( V4 _VectA, V4 _VectB );
    ZENInline float	Dot				( V4 _VectA, V4 _VectB );

    // V4 Operations
    ZENInline V4		LengthV2    	( V4 _Vect );       //Return Magnitude Squared, with same value stored in x,y,z,w    
    ZENInline V4		LengthV			( V4 _Vect );       //Return Magnitude, with same value stored in x,y,z,w
    ZENInline V4		LengthInvV		( V4 _Vect );       //Return 1.0/Magnitude, with same value stored in x,y,z,w
    ZENInline float	Length2			( V4 _Vect );       //Return Magnitude Squared
    ZENInline float	Length			( V4 _Vect );       //Return Magnitude
    ZENInline float	LengthInv		( V4 _Vect );       //Return 1.0/Magnitude of _vA
    ZENInline V4		Normalize		( V4 _Vect );      

	ZENInline bool		IsNearZero		( V4 _Vect );

    extern const V4 v4Zero;
    extern const V4 v4One;
	extern const V4 v4OneNeg;
	extern const V4 v4X1;
	extern const V4 v4Y1;
	extern const V4 v4Z1;
	extern const V4 v4W1;
	extern const V4 v4XYZ1;
	extern const V4 v4Mask3;
	extern const V4 v4MaskW;
}  } // namespace zen, zenMath

#include ZENInlinePlatform( zenMathVector )

#endif
