#pragma once
#ifndef __LibCore_Api_Type_inl__
#define __LibCore_Api_Type_inl__

namespace zen { namespace awtype {

template<>
AWForceInline awRect<awS8>::awRect(void)
: mX(0)
, mY(0)
, mW(0)
, mH(0)
{
}

template<>
AWForceInline awRect<awU8>::awRect(void)
: mX(0)
, mY(0)
, mW(0)
, mH(0)
{
}

template<>
AWForceInline awRect<awS16>::awRect(void)
: mX(0)
, mY(0)
, mW(0)
, mH(0)
{
}

template<>
AWForceInline awRect<awU16>::awRect(void)
: mX(0)
, mY(0)
, mW(0)
, mH(0)
{
}

template<>
AWForceInline awRect<awS32>::awRect(void)
: mX(0)
, mY(0)
, mW(0)
, mH(0)
{
}

template<>
AWForceInline awRect<awU32>::awRect(void)
: mX(0)
, mY(0)
, mW(0)
, mH(0)
{
}

template<>
AWForceInline awRect<awS64>::awRect(void)
: mX(0)
, mY(0)
, mW(0)
, mH(0)
{
}

template<>
AWForceInline awRect<awU64>::awRect(void)
: mX(0)
, mY(0)
, mW(0)
, mH(0)
{
}

template<>
AWForceInline awRect<awF32>::awRect(void)
: mX(0.0f)
, mY(0.0f)
, mW(0.0f)
, mH(0.0f)
{
}

template<>
AWForceInline awRect<awF64>::awRect(void)
: mX(0.0f)
, mY(0.0f)
, mW(0.0f)
, mH(0.0f)
{
}

template<class _Type_>
AWForceInline awRect<_Type_>::awRect(const _Type_& inX, const _Type_& inY, const _Type_& inW, const _Type_& inH)
: mX(inX)
, mY(inY)
, mW(inW)
, mH(inH)
{
}

template<>
AWForceInline bool awRect<awS8>::IsNull(void)
{
	return mX==0 && mY==0 && mW==0 && mH==0;
}

template<>
AWForceInline const awRect<awS8>& awRect<awS8>::SetNull(void)
{
	mX = mY = mW = mH = 0;
	return *this;
}

template<>
AWForceInline bool awRect<awU8>::IsNull(void)
{
	return mX==0 && mY==0 && mW==0 && mH==0;
}

template<>
AWForceInline const awRect<awU8>& awRect<awU8>::SetNull(void)
{
	mX = mY = mW = mH = 0;
	return *this;
}

template<>
AWForceInline bool awRect<awS16>::IsNull(void)
{
	return mX==0 && mY==0 && mW==0 && mH==0;
}

template<>
AWForceInline const awRect<awS16>& awRect<awS16>::SetNull(void)
{
	mX = mY = mW = mH = 0;
	return *this;
}

template<>
AWForceInline bool awRect<awU16>::IsNull(void)
{
	return mX==0 && mY==0 && mW==0 && mH==0;
}

template<>
AWForceInline const awRect<awU16>& awRect<awU16>::SetNull(void)
{
	mX = mY = mW = mH = 0;
	return *this;
}

template<>
AWForceInline bool awRect<awS32>::IsNull(void)
{
	return mX==0 && mY==0 && mW==0 && mH==0;
}

template<>
AWForceInline const awRect<awS32>& awRect<awS32>::SetNull(void)
{
	mX = mY = mW = mH = 0;
	return *this;
}

template<>
AWForceInline bool awRect<awU32>::IsNull(void)
{
	return mX==0 && mY==0 && mW==0 && mH==0;
}

template<>
AWForceInline const awRect<awU32>& awRect<awU32>::SetNull(void)
{
	mX = mY = mW = mH = 0;
	return *this;
}

template<>
AWForceInline bool awRect<awS64>::IsNull(void)
{
	return mX==0 && mY==0 && mW==0 && mH==0;
}

template<>
AWForceInline const awRect<awS64>& awRect<awS64>::SetNull(void)
{
	mX = mY = mW = mH = 0;
	return *this;
}

template<>
AWForceInline bool awRect<awU64>::IsNull(void)
{
	return mX==0 && mY==0 && mW==0 && mH==0;
}

template<>
AWForceInline const awRect<awU64>& awRect<awU64>::SetNull(void)
{
	mX = mY = mW = mH = 0;
	return *this;
}

template<>
AWForceInline bool awRect<awF32>::IsNull(void)
{
	return mX==0.0f && mY==0.0f && mW==0.0f && mH==0.0f;
}

template<>
AWForceInline const awRect<awF32>& awRect<awF32>::SetNull(void)
{
	mX = mY = mW = mH = 0.0f;
	return *this;
}

template<>
AWForceInline bool awRect<awF64>::IsNull(void)
{
	return mX==0.0 && mY==0.0 && mW==0.0 && mH==0.0;
}

template<>
AWForceInline const awRect<awF64>& awRect<awF64>::SetNull(void)
{
	mX = mY = mW = mH = 0.0;
	return *this;
}

template<class _Type_>
AWForceInline awRect<_Type_>::awRect(const awRect& xOther)
	: mX(xOther.mX)
	, mY(xOther.mY)
	, mW(xOther.mW)
	, mH(xOther.mH)
{
}

}}//namespace zen { namespace awtype {

#endif //__LibCore_Api_Type_inl__
