#pragma once

#include <xmmintrin.h>

namespace zen { namespace zenMath 
{
    typedef __m128 V4;

    // Values Set
    zenInline V4		simdPoint       ( float _fValue         );
    zenInline V4		simdPoint       ( const float* _pValues );
    zenInline V4		simdPoint       ( const float _fX, const float _fY, const float _fZ);
    zenInline V4		simdVector      ( float _fValue         );
    zenInline V4		simdVector      ( const float* _pValues );
    zenInline V4		simdVector      ( const float _fX, const float _fY, const float _fZ);

    zenInline V4		simdXYZW		( float _fValue         );
    zenInline V4		simdXYZW		( const float* _pValues );
    zenInline V4		simdXYZW		( float _fX, float _fY, float _fZ, float _fW);

	/*
	zenInline V4		XYZW            ( zU32 _uValue         );
	zenInline V4		XYZW            ( const zU32* _uValues );
	zenInline V4		XYZW            ( zU32 _uX, zU32 _uY, zU32 _uZ, zU32 _uW);
	*/
    zenInline V4		simdSpreadX         ( V4 _Vect );
    zenInline V4		simdSpreadY         ( V4 _Vect );
    zenInline V4		simdSpreadZ         ( V4 _Vect );
    zenInline V4		simdSpreadW         ( V4 _Vect );    
  //  zenInline V4  Shuffle         ( V4 _Vect, int _uMask);              //.xyzw = Values picked from _Vect.xyzw, moved around (use macro SHUFFLE_MASK)
  //  zenInline V4  Shuffle         ( V4 _VectA, V4 _VectB, int _Mask);   //.xy = 2 values picked from _VectA.xyzw and .zw = 2 values from _VectB.xyzw (use macro SHUFFLE_MASK)

    // Values Get
    zenInline void		simdXYZW        ( V4 _Vect, float* _pValuesOut );
    zenInline float		simdX           ( V4 _Vect );
    zenInline float		simdY           ( V4 _Vect );
    zenInline float		simdZ           ( V4 _Vect );
    zenInline float		simdW           ( V4 _Vect );    
    
	// Arithmerics
    zenInline V4		simdAdd			( V4 _VectA, V4 _VectB );
    zenInline V4		simdSub			( V4 _VectA, V4 _VectB );
    zenInline V4		simdMul			( V4 _VectA, V4 _VectB );
    zenInline V4		simdDiv			( V4 _VectA, V4 _VectB );
    zenInline V4		simdMAdd		( V4 _VectA, V4 _VectB, V4 _VectC);  //(A*B)+C
    zenInline V4		simdMax			( V4 _VectA, V4 _VectB );
    zenInline V4		simdMin			( V4 _VectA, V4 _VectB );    
    zenInline V4		simdCross		( V4 _VectA, V4 _VectB );
    zenInline V4		simdDotV		( V4 _VectA, V4 _VectB );
    zenInline float		simdDot			( V4 _VectA, V4 _VectB );

    // V4 Operations
    zenInline V4		simdLengthV2    ( V4 _Vect );       //Return Magnitude Squared, with same value stored in x,y,z,w    
    zenInline V4		simdLengthV		( V4 _Vect );       //Return Magnitude, with same value stored in x,y,z,w
    zenInline V4		simdLengthInvV	( V4 _Vect );       //Return 1.0/Magnitude, with same value stored in x,y,z,w
    zenInline float		simdLength2		( V4 _Vect );       //Return Magnitude Squared
    zenInline float		simdLength		( V4 _Vect );       //Return Magnitude
    zenInline float		simdLengthInv	( V4 _Vect );       //Return 1.0/Magnitude of _vA
    zenInline V4		simdNormalize	( V4 _Vect );      

	zenInline bool		simdIsNearZero	( V4 _Vect );

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

#include zenInlinePlatform( zenBaseMathVector )
