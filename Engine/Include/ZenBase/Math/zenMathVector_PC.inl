#include <pmmintrin.h>

#define SHUFFLE_MASK( _X1_, _Y1_, _Z2_, _W2_ ) ( ((_X1_)<<0) | ((_Y1_)<<2) | ((_Z2_)<<4) | ((_W2_)<<6) )    //0:x 1:y 2:z 3:w

namespace zen { namespace zenMath 
{

//=================================================================================================
// V4 : Construction
//=================================================================================================
V4 Point( const float* _pValues )												{ return _mm_set_ps(1.0f, _pValues[2], _pValues[1], _pValues[0]); }
V4 Point( float _fValue )														{ return _mm_set_ps(1.0f, _fValue, _fValue, _fValue); }
V4 Point( const float _fX, const float _fY, const float _fZ)					{ return _mm_set_ps(1.0f, _fZ, _fY, _fX); }
V4 Vector( const float* _pValues )												{ return _mm_set_ps(0.0f, _pValues[2], _pValues[1], _pValues[0]); }
V4 Vector( float _fValue )														{ return _mm_set_ps(0.0f, _fValue, _fValue, _fValue); }
V4 Vector( const float _fX, const float _fY, const float _fZ)					{ return _mm_set_ps(0.0f, _fZ, _fY, _fX); }	

V4 XYZW( const float* _pValues )												{ return _mm_load_ps(_pValues); }//@TODO: Add Assert alignement 
V4 XYZW( float _fValue )														{ return _mm_set_ps(_fValue, _fValue, _fValue, _fValue); }
V4 XYZW( float _fX, float _fY, float _fZ, float _fW)							{ return _mm_set_ps(_fW, _fZ, _fY, _fX); }

V4 SpreadX( V4 _Vect )															{ return _mm_shuffle_ps(_Vect, _Vect, SHUFFLE_MASK(0,0,0,0) ); }
V4 SpreadY( V4 _Vect )															{ return _mm_shuffle_ps(_Vect, _Vect, SHUFFLE_MASK(1,1,1,1)); }
V4 SpreadZ( V4 _Vect )															{ return _mm_shuffle_ps(_Vect, _Vect, SHUFFLE_MASK(2,2,2,2)); }
V4 SpreadW( V4 _Vect )															{ return _mm_shuffle_ps(_Vect, _Vect, SHUFFLE_MASK(3,3,3,3)); }

//V4 Shuffle( V4 _Vect, int _Mask)                             { return _mm_shuffle_ps(_Vect, _Vect, _Mask); }
//V4 Shuffle( V4 _VectA, V4 _VectB, int _Mask)                 { return _mm_shuffle_ps(_VectA, _VectB, _Mask); }

//=================================================================================================
// V4 : Values retrieval
//=================================================================================================
void XYZW( V4 _Vect, float* _pValuesOut ) 
{ 
    //@TODO: Add Assert alignement
    _mm_store_ps(_pValuesOut, _Vect); 
}

float X( V4 _Vect )           { return _Vect.m128_f32[0]; }
float Y( V4 _Vect )           { return _Vect.m128_f32[1]; }
float Z( V4 _Vect )           { return _Vect.m128_f32[2]; }
float W( V4 _Vect )           { return _Vect.m128_f32[3]; }

//=================================================================================================
// V4 : Arithmetic
//=================================================================================================
V4 Add( V4 _VectA, V4 _VectB )								{ return _mm_add_ps(_VectA, _VectB); }
V4 Sub( V4 _VectA, V4 _VectB )								{ return _mm_sub_ps(_VectA, _VectB); }
V4 Mul( V4 _VectA, V4 _VectB )								{ return _mm_mul_ps(_VectA, _VectB); }
V4 Div( V4 _VectA, V4 _VectB )								{ return _mm_div_ps(_VectA, _VectB); }
V4 MAdd( V4 _VectA, V4 _VectB, V4 _VectC)					{ return _mm_add_ps(_mm_mul_ps(_VectA, _VectB), _VectC); }
V4 Max( V4 _VectA, V4 _VectB )								{ return _mm_max_ps(_VectA, _VectB); }
V4 Min( V4 _VectA, V4 _VectB )								{ return _mm_min_ps(_VectA, _VectB); }
   
V4	Cross( V4 _VectA, V4 _VectB )
{
    V4 vValueA, vValueB, vResult;
    vValueA = _mm_shuffle_ps(_VectA, _VectA, SHUFFLE_MASK(1,2,0,3) );   //(A.y,A.z,A.x,A.w)
    vValueB = _mm_shuffle_ps(_VectB, _VectB, SHUFFLE_MASK(2,0,1,3) );   //(B.z,B.x,B.y,B.w)
    vResult = _mm_mul_ps(vValueA, vValueB);                             

    vValueA = _mm_shuffle_ps(_VectA, _VectA, SHUFFLE_MASK(2,0,1,3) );   //(A.z,A.x,A.y,A.w)
    vValueB = _mm_shuffle_ps(_VectB, _VectB, SHUFFLE_MASK(1,2,0,3) );   //(B.y,B.z,B.x,A.w)
    return _mm_sub_ps( vResult, _mm_mul_ps(vValueA, vValueB) );
	
}

V4 DotV( V4 _VectA, V4 _VectB )
{
	V4 vResult	= _mm_mul_ps(_VectA, _VectB); 
	vResult		= _mm_hadd_ps(vResult, vResult);
	return _mm_hadd_ps(vResult, vResult);
}

float Dot( V4 _VectA, V4 _VectB )
{
	return DotV(_VectA, _VectB).m128_f32[0];
}

V4 LengthV2( V4 _VectA )
{
    V4 vSquared	= _mm_mul_ps(_VectA, _VectA);
    vSquared	= _mm_hadd_ps(vSquared, vSquared);
    return		_mm_hadd_ps(vSquared, vSquared);
}

V4 LengthV( V4 _VectA )
{
    V4 vSquared	= _mm_mul_ps(_VectA, _VectA);
    vSquared	= _mm_hadd_ps(vSquared, vSquared);
    vSquared	= _mm_hadd_ps(vSquared, vSquared);
    return _mm_sqrt_ps(vSquared);
}

V4 LengthInvV( V4 _VectA )
{
    V4 vSquared	= _mm_mul_ps(_VectA, _VectA);
    vSquared	= _mm_hadd_ps(vSquared, vSquared);
    vSquared	= _mm_hadd_ps(vSquared, vSquared);
    return _mm_rsqrt_ps(vSquared);
}

float Length2( V4 _VectA )           
{
    return LengthV2(_VectA).m128_f32[0];    
}

float Length( V4 _VectA )           
{
    return LengthV(_VectA).m128_f32[0];    
}

float LengthInv( V4 _VectA )
{
    return LengthInvV(_VectA).m128_f32[0];    
}

ZENInline V4 Normalize( V4 _VectA )
{
	V4 vLength = LengthV(_VectA);
    return _mm_div_ps(_VectA, vLength);
}

ZENInline bool	IsNearZero( V4 _Vect )
{
	return Dot( _Vect, _Vect ) < zenConst::kfSmallNumber;
}

}  } // namespace zen, zenMath
