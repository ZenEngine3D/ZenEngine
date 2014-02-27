#pragma once
#ifndef __LibZenBase_Api_Math_Vector4_h__
#define __LibZenBase_Api_Math_Vector4_h__

#include <xmmintrin.h>

namespace zen { namespace zenMath 
{
    typedef __m128 V4;

    // Values Set
    inline V4		Point           ( float _fValue         );
    inline V4		Point           ( const float* _pValues );
    inline V4		Point           ( const float _fX, const float _fY, const float _fZ);
    inline V4		Vector          ( float _fValue         );
    inline V4		Vector          ( const float* _pValues );
    inline V4		Vector          ( const float _fX, const float _fY, const float _fZ);

    inline V4		XYZW            ( float _fValue         );
    inline V4		XYZW            ( const float* _pValues );
    inline V4		XYZW            ( float _fX, float _fY, float _fZ, float _fW);
	/*
	inline V4		XYZW            ( zenU32 _uValue         );
	inline V4		XYZW            ( const zenU32* _uValues );
	inline V4		XYZW            ( zenU32 _uX, zenU32 _uY, zenU32 _uZ, zenU32 _uW);
	*/
    inline V4		SpreadX         ( V4 _Vect );
    inline V4		SpreadY         ( V4 _Vect );
    inline V4		SpreadZ         ( V4 _Vect );
    inline V4		SpreadW         ( V4 _Vect );    
  //  inline V4  Shuffle         ( V4 _Vect, int _uMask);              //.xyzw = Values picked from _Vect.xyzw, moved around (use macro SHUFFLE_MASK)
  //  inline V4  Shuffle         ( V4 _VectA, V4 _VectB, int _Mask);   //.xy = 2 values picked from _VectA.xyzw and .zw = 2 values from _VectB.xyzw (use macro SHUFFLE_MASK)

    // Values Get
    inline void		XYZW            ( V4 _Vect, float* _pValuesOut );
    inline float	X               ( V4 _Vect );
    inline float	Y               ( V4 _Vect );
    inline float	Z               ( V4 _Vect );
    inline float	W               ( V4 _Vect );    
    
	// Arithmerics
    inline V4		Add				( V4 _VectA, V4 _VectB );
    inline V4		Sub				( V4 _VectA, V4 _VectB );
    inline V4		Mul				( V4 _VectA, V4 _VectB );
    inline V4		Div				( V4 _VectA, V4 _VectB );
    inline V4		MAdd			( V4 _VectA, V4 _VectB, V4 _VectC);  //(A*B)+C
    inline V4		Max				( V4 _VectA, V4 _VectB );
    inline V4		Min				( V4 _VectA, V4 _VectB );    
    inline V4		Cross			( V4 _VectA, V4 _VectB );
    inline V4		DotV			( V4 _VectA, V4 _VectB );
    inline float	Dot				( V4 _VectA, V4 _VectB );

    // V4 Operations
    inline V4		LengthV2    	( V4 _Vect );       //Return Magnitude Squared, with same value stored in x,y,z,w    
    inline V4		LengthV			( V4 _Vect );       //Return Magnitude, with same value stored in x,y,z,w
    inline V4		LengthInvV		( V4 _Vect );       //Return 1.0/Magnitude, with same value stored in x,y,z,w
    inline float	Length2			( V4 _Vect );       //Return Magnitude Squared
    inline float	Length			( V4 _Vect );       //Return Magnitude
    inline float	LengthInv		( V4 _Vect );       //Return 1.0/Magnitude of _vA
    inline V4		Normalize		( V4 _Vect );      

	inline bool		IsNearZero		( V4 _Vect );

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
