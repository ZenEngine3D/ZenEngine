#pragma once
#ifndef __zenApi_Base_Math_Base_h__
#define __zenApi_Base_Math_Base_h__ 

#include <math.h> //Used for modf

namespace zen { namespace zenMath 
{
	template<class _Type_>
	_Type_ DivRoundUp( const _Type_& _Numerator, const _Type_& _Denominator ) 
	{
		return (_Numerator+_Denominator-1)/_Denominator; 
	}

	template<class _Type_>
	_Type_ Min( const _Type_& _Value1, const _Type_& _Value2 ) 
	{
		return _Value1 <= _Value2 ? _Value1 : _Value2; 
	}

	template<class _Type_>
	_Type_ Max( const _Type_& _Value1, const _Type_& _Value2 ) 
	{
		return _Value1 >= _Value2 ? _Value1 : _Value2; 
	}
		
	template<class _Type_>
	_Type_ Clamp( const _Type_& _Value, const _Type_& _Min, const _Type_& _Max ) 
	{
		return (_Value >= _Max) ? _Max : (_Value <= _Min) ? _Min : _Value; 
	}

	template<class _Type_>
	_Type_ Lerp( const _Type_& _StartValue, const _Type_& _EndValue, float _fRatio ) 
	{
		float fRatio = Clamp(_fRatio, 0.f, 1.f);
		return _StartValue*(1.0f-fRatio) + _EndValue*fRatio; 
	}

	template<class _Type_>
	_Type_ TriLerp( const _Type_& _StartValue, const _Type_& _MidValue, const _Type_& _EndValue, float _fRatio ) 
	{
		float fRatioClamp	= Clamp(_fRatio, 0.f, 1.f);
		float fRatio		= (fRatioClamp < 0.5f) ? fRatioClamp*2.f : (fRatioClamp-0.5f)*2.f;
		return (fRatioClamp < 0.5f)	? _StartValue*(1.0f-fRatio) + _MidValue*fRatio
									: _MidValue*(1.0f-fRatio)	+ _EndValue*fRatio; 
	}

	zenInline float Fract(float _fValue){ float fIntPart; return modf(_fValue, &fIntPart); }

	zenInline bool IsPower2(zU32 x){ return ( (x > 0) && ((x & (x - 1)) == 0) ); }
	zenInline bool IsPower2(zU64 x){ return ( (x > 0) && ((x & (x - 1)) == 0) ); }
	zenInline bool IsPower2(zI32 x){ return ( (x > 0) && ((x & (x - 1)) == 0) ); }
	zenInline bool IsPower2(zI64 x){ return ( (x > 0) && ((x & (x - 1)) == 0) ); }

	zenInline zUInt RoundPow2( zUInt _Value, zUInt _Pow2Multiple )
	{
		zenAssert(IsPower2(_Pow2Multiple));
		return (_Value + (_Pow2Multiple-1)) & ~(_Pow2Multiple-1);
	}

	template<class TType>
	zenInline TType RoundUp( TType _uSize, TType _uMultiple )
	{
		return ((_uSize+_uMultiple-1) / _uMultiple) * _uMultiple;
	}

	const float kfPI		= 3.141592653589793f;
	const float kfPI2x		= 6.283185307179586f;
	const float kfPIOn2		= 1.570796326794896f;
	const float kfPIInv		= 1.0f/3.141592653589793f;
	const float kfPI2xInv	= 1.0f/6.283185307179586f;
	const float kfPIOn2Inv	= 1.0f/1.570796326794896f;
	const double kdPI		= 3.141592653589793;
	const double kdPI2x		= 6.283185307179586;
	const double kdPIOn2	= 1.570796326794896;
	const double kdPIInv	= 1.0/3.141592653589793;
	const double kdPI2xInv	= 1.0/6.283185307179586;
	const double kdPIOn2Inv	= 1.0/1.570796326794896;

}  } // namespace zen, zenMath

#endif
