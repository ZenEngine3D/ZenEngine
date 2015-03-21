#pragma once
#ifndef __zenApi_Base_Math_Vector4_h__
#define __zenApi_Base_Math_Vector4_h__

#include <xmmintrin.h>

namespace zen { namespace zenMath 
{
    typedef __m128 V4;

    // Values Set
    ZENInline V4		simdPoint       ( float _fValue         );
    ZENInline V4		simdPoint       ( const float* _pValues );
    ZENInline V4		simdPoint       ( const float _fX, const float _fY, const float _fZ);
    ZENInline V4		simdVector      ( float _fValue         );
    ZENInline V4		simdVector      ( const float* _pValues );
    ZENInline V4		simdVector      ( const float _fX, const float _fY, const float _fZ);

    ZENInline V4		simdXYZW		( float _fValue         );
    ZENInline V4		simdXYZW		( const float* _pValues );
    ZENInline V4		simdXYZW		( float _fX, float _fY, float _fZ, float _fW);
	/*
	ZENInline V4		XYZW            ( zU32 _uValue         );
	ZENInline V4		XYZW            ( const zU32* _uValues );
	ZENInline V4		XYZW            ( zU32 _uX, zU32 _uY, zU32 _uZ, zU32 _uW);
	*/
    ZENInline V4		simdSpreadX         ( V4 _Vect );
    ZENInline V4		simdSpreadY         ( V4 _Vect );
    ZENInline V4		simdSpreadZ         ( V4 _Vect );
    ZENInline V4		simdSpreadW         ( V4 _Vect );    
  //  ZENInline V4  Shuffle         ( V4 _Vect, int _uMask);              //.xyzw = Values picked from _Vect.xyzw, moved around (use macro SHUFFLE_MASK)
  //  ZENInline V4  Shuffle         ( V4 _VectA, V4 _VectB, int _Mask);   //.xy = 2 values picked from _VectA.xyzw and .zw = 2 values from _VectB.xyzw (use macro SHUFFLE_MASK)

    // Values Get
    ZENInline void		simdXYZW        ( V4 _Vect, float* _pValuesOut );
    ZENInline float		simdX           ( V4 _Vect );
    ZENInline float		simdY           ( V4 _Vect );
    ZENInline float		simdZ           ( V4 _Vect );
    ZENInline float		simdW           ( V4 _Vect );    
    
	// Arithmerics
    ZENInline V4		simdAdd			( V4 _VectA, V4 _VectB );
    ZENInline V4		simdSub			( V4 _VectA, V4 _VectB );
    ZENInline V4		simdMul			( V4 _VectA, V4 _VectB );
    ZENInline V4		simdDiv			( V4 _VectA, V4 _VectB );
    ZENInline V4		simdMAdd		( V4 _VectA, V4 _VectB, V4 _VectC);  //(A*B)+C
    ZENInline V4		simdMax			( V4 _VectA, V4 _VectB );
    ZENInline V4		simdMin			( V4 _VectA, V4 _VectB );    
    ZENInline V4		simdCross		( V4 _VectA, V4 _VectB );
    ZENInline V4		simdDotV		( V4 _VectA, V4 _VectB );
    ZENInline float		simdDot			( V4 _VectA, V4 _VectB );

    // V4 Operations
    ZENInline V4		simdLengthV2    ( V4 _Vect );       //Return Magnitude Squared, with same value stored in x,y,z,w    
    ZENInline V4		simdLengthV		( V4 _Vect );       //Return Magnitude, with same value stored in x,y,z,w
    ZENInline V4		simdLengthInvV	( V4 _Vect );       //Return 1.0/Magnitude, with same value stored in x,y,z,w
    ZENInline float		simdLength2		( V4 _Vect );       //Return Magnitude Squared
    ZENInline float		simdLength		( V4 _Vect );       //Return Magnitude
    ZENInline float		simdLengthInv	( V4 _Vect );       //Return 1.0/Magnitude of _vA
    ZENInline V4		simdNormalize	( V4 _Vect );      

	ZENInline bool		simdIsNearZero	( V4 _Vect );

    extern const V4		v4Zero;
    extern const V4		v4One;
	extern const V4		v4OneNeg;
	extern const V4		v4X1;
	extern const V4		v4Y1;
	extern const V4		v4Z1;
	extern const V4		v4W1;
	extern const V4		v4XYZ1;
	extern const V4		v4Mask3;
	extern const V4		v4MaskW;
}  } // namespace zen, zenMath

#include ZENInlinePlatform( zenBaseMathVector )

#endif
