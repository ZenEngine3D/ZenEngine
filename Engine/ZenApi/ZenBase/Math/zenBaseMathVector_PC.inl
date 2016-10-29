#include <pmmintrin.h>

#define SHUFFLE_MASK( _X1_, _Y1_, _Z2_, _W2_ ) ( ((_X1_)<<0) | ((_Y1_)<<2) | ((_Z2_)<<4) | ((_W2_)<<6) )    //0:x 1:y 2:z 3:w

namespace zen { namespace zenMath 
{

//=================================================================================================
// V4 : Construction
//=================================================================================================
V4 simdPoint( const float* _pValues )												{ return _mm_set_ps(1.0f, _pValues[2], _pValues[1], _pValues[0]); }
V4 simdPoint( float _fValue )														{ return _mm_set_ps(1.0f, _fValue, _fValue, _fValue); }
V4 simdPoint( const float _fX, const float _fY, const float _fZ)					{ return _mm_set_ps(1.0f, _fZ, _fY, _fX); }
V4 simdVector( const float* _pValues )												{ return _mm_set_ps(0.0f, _pValues[2], _pValues[1], _pValues[0]); }
V4 simdVector( float _fValue )														{ return _mm_set_ps(0.0f, _fValue, _fValue, _fValue); }
V4 simdVector( const float _fX, const float _fY, const float _fZ)					{ return _mm_set_ps(0.0f, _fZ, _fY, _fX); }	

V4 simdXYZW( const float* _pValues )												{ return _mm_load_ps(_pValues); }//! @todo Add Assert alignement 
V4 simdXYZW( float _fValue )														{ return _mm_set_ps(_fValue, _fValue, _fValue, _fValue); }
V4 simdXYZW( float _fX, float _fY, float _fZ, float _fW)							{ return _mm_set_ps(_fW, _fZ, _fY, _fX); }

V4 simdSpreadX( V4 _Vect )															{ return _mm_shuffle_ps(_Vect, _Vect, SHUFFLE_MASK(0,0,0,0) ); }
V4 simdSpreadY( V4 _Vect )															{ return _mm_shuffle_ps(_Vect, _Vect, SHUFFLE_MASK(1,1,1,1)); }
V4 simdSpreadZ( V4 _Vect )															{ return _mm_shuffle_ps(_Vect, _Vect, SHUFFLE_MASK(2,2,2,2)); }
V4 simdSpreadW( V4 _Vect )															{ return _mm_shuffle_ps(_Vect, _Vect, SHUFFLE_MASK(3,3,3,3)); }

//V4 Shuffle( V4 _Vect, int _Mask)                             { return _mm_shuffle_ps(_Vect, _Vect, _Mask); }
//V4 Shuffle( V4 _VectA, V4 _VectB, int _Mask)                 { return _mm_shuffle_ps(_VectA, _VectB, _Mask); }

//=================================================================================================
// V4 : Values retrieval
//=================================================================================================
void simdXYZW( V4 _Vect, float* _pValuesOut ) 
{ 
    //! @todo Add Assert alignement
    _mm_store_ps(_pValuesOut, _Vect); 
}

float simdX( V4 _Vect )           { return _Vect.m128_f32[0]; }
float simdY( V4 _Vect )           { return _Vect.m128_f32[1]; }
float simdZ( V4 _Vect )           { return _Vect.m128_f32[2]; }
float simdW( V4 _Vect )           { return _Vect.m128_f32[3]; }

//=================================================================================================
// V4 : Arithmetic
//=================================================================================================
V4 simdAdd( V4 _VectA, V4 _VectB )								{ return _mm_add_ps(_VectA, _VectB); }
V4 simdSub( V4 _VectA, V4 _VectB )								{ return _mm_sub_ps(_VectA, _VectB); }
V4 simdMul( V4 _VectA, V4 _VectB )								{ return _mm_mul_ps(_VectA, _VectB); }
V4 simdDiv( V4 _VectA, V4 _VectB )								{ return _mm_div_ps(_VectA, _VectB); }
V4 simdMAdd( V4 _VectA, V4 _VectB, V4 _VectC)					{ return _mm_add_ps(_mm_mul_ps(_VectA, _VectB), _VectC); }
V4 simdMax( V4 _VectA, V4 _VectB )								{ return _mm_max_ps(_VectA, _VectB); }
V4 simdMin( V4 _VectA, V4 _VectB )								{ return _mm_min_ps(_VectA, _VectB); }
   
V4	simdCross( V4 _VectA, V4 _VectB )
{
    V4 vValueA, vValueB, vResult;
    vValueA = _mm_shuffle_ps(_VectA, _VectA, SHUFFLE_MASK(1,2,0,3) );   //(A.y,A.z,A.x,A.w)
    vValueB = _mm_shuffle_ps(_VectB, _VectB, SHUFFLE_MASK(2,0,1,3) );   //(B.z,B.x,B.y,B.w)
    vResult = _mm_mul_ps(vValueA, vValueB);                             

    vValueA = _mm_shuffle_ps(_VectA, _VectA, SHUFFLE_MASK(2,0,1,3) );   //(A.z,A.x,A.y,A.w)
    vValueB = _mm_shuffle_ps(_VectB, _VectB, SHUFFLE_MASK(1,2,0,3) );   //(B.y,B.z,B.x,A.w)
    return _mm_sub_ps( vResult, _mm_mul_ps(vValueA, vValueB) );
	
}

V4 simdDotV( V4 _VectA, V4 _VectB )
{
	V4 vResult	= _mm_mul_ps(_VectA, _VectB); 
	vResult		= _mm_hadd_ps(vResult, vResult);
	return _mm_hadd_ps(vResult, vResult);
}

float simdDot( V4 _VectA, V4 _VectB )
{
	return simdDotV(_VectA, _VectB).m128_f32[0];
}

V4 simdLengthV2( V4 _VectA )
{
    V4 vSquared	= _mm_mul_ps(_VectA, _VectA);
    vSquared	= _mm_hadd_ps(vSquared, vSquared);
    return		_mm_hadd_ps(vSquared, vSquared);
}

V4 simdLengthV( V4 _VectA )
{
    V4 vSquared	= _mm_mul_ps(_VectA, _VectA);
    vSquared	= _mm_hadd_ps(vSquared, vSquared);
    vSquared	= _mm_hadd_ps(vSquared, vSquared);
    return _mm_sqrt_ps(vSquared);
}

V4 simdLengthInvV( V4 _VectA )
{
    V4 vSquared	= _mm_mul_ps(_VectA, _VectA);
    vSquared	= _mm_hadd_ps(vSquared, vSquared);
    vSquared	= _mm_hadd_ps(vSquared, vSquared);
    return _mm_rsqrt_ps(vSquared);
}

float simdLength2( V4 _VectA )           
{
    return simdLengthV2(_VectA).m128_f32[0];    
}

float simdLength( V4 _VectA )           
{
    return simdLengthV(_VectA).m128_f32[0];    
}

float simdLengthInv( V4 _VectA )
{
    return simdLengthInvV(_VectA).m128_f32[0];    
}

zenInline V4 simdNormalize( V4 _VectA )
{
	V4 vLength = simdLengthV(_VectA);
    return _mm_div_ps(_VectA, vLength);
}

zenInline bool	simdIsNearZero( V4 _Vect )
{
	return simdDot( _Vect, _Vect ) < zenConst::kfSmallNumber;
}

}  } // namespace zen, zenMath
