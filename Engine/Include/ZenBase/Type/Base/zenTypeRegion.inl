#pragma once
#ifndef __LibCore_Api_Type_Region_inl__
#define __LibCore_Api_Type_Region_inl__

namespace zen { namespace awtype {

template<>
AWForceInline awRegion<awS8>::awRegion(void)
: mX1(0)
, mY1(0)
, mX2(0)
, mY2(0)
{
}

template<>
AWForceInline awRegion<awU8>::awRegion(void)
: mX1(0)
, mY1(0)
, mX2(0)
, mY2(0)
{
}

template<>
AWForceInline awRegion<awS16>::awRegion(void)
: mX1(0)
, mY1(0)
, mX2(0)
, mY2(0)
{
}

template<>
AWForceInline awRegion<awU16>::awRegion(void)
: mX1(0)
, mY1(0)
, mX2(0)
, mY2(0)
{
}

template<>
AWForceInline awRegion<awS32>::awRegion(void)
: mX1(0)
, mY1(0)
, mX2(0)
, mY2(0)
{
}

template<>
AWForceInline awRegion<awU32>::awRegion(void)
: mX1(0)
, mY1(0)
, mX2(0)
, mY2(0)
{
}

template<>
AWForceInline awRegion<awS64>::awRegion(void)
: mX1(0)
, mY1(0)
, mX2(0)
, mY2(0)
{
}

template<>
AWForceInline awRegion<awU64>::awRegion(void)
: mX1(0)
, mY1(0)
, mX2(0)
, mY2(0)
{
}

template<>
AWForceInline awRegion<awF32>::awRegion(void)
: mX1(0.0f)
, mY1(0.0f)
, mX2(0.0f)
, mY2(0.0f)
{
}

template<>
AWForceInline awRegion<awF64>::awRegion(void)
: mX1(0.0f)
, mY1(0.0f)
, mX2(0.0f)
, mY2(0.0f)
{
}

template<class _Type_>
AWForceInline awRegion<_Type_>::awRegion(const _Type_ inX1, const _Type_ inY1, const _Type_ inX2, const _Type_ inY2)
: mX1(inX1)
, mY1(inY1)
, mX2(inX2)
, mY2(inY2)
{
}

template<class _Type_>
AWForceInline awRegion<_Type_>::awRegion(const awRegion& xOther)
: mX1(xOther.mX1)
, mY1(xOther.mY1)
, mX2(xOther.mX2)
, mY2(xOther.mY2)
{
}

template<class _Type_>
AWForceInline bool awRegion<_Type_>::operator!=(const awRegion& xOther)
{
	return (mX1 != xOther.mX1 || mY1 != xOther.mY1 || mX2 != xOther.mX2 || mY2 != xOther.mY2);
}

template<class _Type_>
AWForceInline void awRegion<_Type_>::Intersect(const awRegion& xBox1, const awRegion& xBox2)
{
	mX1 = std::max(xBox1.mX1, xBox2.mX1); mY1 = std::max(xBox1.mY1, xBox2.mY1);
	mX2 = std::min(xBox1.mX2, xBox2.mX2); mY2 = std::min(xBox1.mY2, xBox2.mY2);
}

template<class _Type_>
AWForceInline void awRegion<_Type_>::Intersect(const awRegion& xBox)
{
	mX1 = std::max(xBox.mX1, mX1); mY1 = std::max(xBox.mY1, mY1);
	mX2 = std::min(xBox.mX2, mX2); mY2 = std::min(xBox.mY2, mY2);
}

template<class _Type_>
AWForceInline bool awRegion<_Type_>::IsInside(_Type_ inX, _Type_ inY)
{
	return (mX1 <= inX && mX2 >= inX && mY1 <= inY && mY2 >= inY);
}

template<class _Type_>
AWForceInline void awRegion<_Type_>::Offset(const awVector2<_Type_>* pxOffset)
{
	mX1 += pxOffset->mX1; mX2 += pxOffset->mX1;
	mX1 += pxOffset->mY1; mX2 += pxOffset->mY1;
}

template<class _Type_>
AWForceInline bool awRegion<_Type_>::IsInside(const awVector2<_Type_>* pxPos)
{
	return (mX1 <= pxPos->mX1 && mX2 >= pxPos->mX1 && mY1 <= pxPos->mY1 && mY2 >= pxPos->mY1);
}

template<>
AWForceInline bool awRegion<awS8>::IsNull(void)
{
	return mX1==0 && mY1==0 && mX2==0 && mY2==0;
}

template<>
AWForceInline const awRegion<awS8>& awRegion<awS8>::SetNull(void)
{
	mX1 = mY1 = mX2 = mY2 = 0;
	return *this;
}

template<>
AWForceInline bool awRegion<awU8>::IsNull(void)
{
	return mX1==0 && mY1==0 && mX2==0 && mY2==0;
}

template<>
AWForceInline const awRegion<awU8>& awRegion<awU8>::SetNull(void)
{
	mX1 = mY1 = mX2 = mY2 = 0;
	return *this;
}

template<>
AWForceInline bool awRegion<awS16>::IsNull(void)
{
	return mX1==0 && mY1==0 && mX2==0 && mY2==0;
}

template<>
AWForceInline const awRegion<awS16>& awRegion<awS16>::SetNull(void)
{
	mX1 = mY1 = mX2 = mY2 = 0;
	return *this;
}

template<>
AWForceInline bool awRegion<awU16>::IsNull(void)
{
	return mX1==0 && mY1==0 && mX2==0 && mY2==0;
}

template<>
AWForceInline const awRegion<awU16>& awRegion<awU16>::SetNull(void)
{
	mX1 = mY1 = mX2 = mY2 = 0;
	return *this;
}

template<>
AWForceInline bool awRegion<awS32>::IsNull(void)
{
	return mX1==0 && mY1==0 && mX2==0 && mY2==0;
}

template<>
AWForceInline const awRegion<awS32>& awRegion<awS32>::SetNull(void)
{
	mX1 = mY1 = mX2 = mY2 = 0;
	return *this;
}

template<>
AWForceInline bool awRegion<awU32>::IsNull(void)
{
	return mX1==0 && mY1==0 && mX2==0 && mY2==0;
}

template<>
AWForceInline const awRegion<awU32>& awRegion<awU32>::SetNull(void)
{
	mX1 = mY1 = mX2 = mY2 = 0;
	return *this;
}

template<>
AWForceInline bool awRegion<awS64>::IsNull(void)
{
	return mX1==0 && mY1==0 && mX2==0 && mY2==0;
}

template<>
AWForceInline const awRegion<awS64>& awRegion<awS64>::SetNull(void)
{
	mX1 = mY1 = mX2 = mY2 = 0;
	return *this;
}

template<>
AWForceInline bool awRegion<awU64>::IsNull(void)
{
	return mX1==0 && mY1==0 && mX2==0 && mY2==0;
}

template<>
AWForceInline const awRegion<awU64>& awRegion<awU64>::SetNull(void)
{
	mX1 = mY1 = mX2 = mY2 = 0;
	return *this;
}

template<>
AWForceInline bool awRegion<awF32>::IsNull(void)
{
	return mX1==0.0f && mY1==0.0f && mX2==0.0f && mY2==0.0f;
}

template<>
AWForceInline const awRegion<awF32>& awRegion<awF32>::SetNull(void)
{
	mX1 = mY1 = mX2 = mY2 = 0.0f;
	return *this;
}

template<>
AWForceInline bool awRegion<awF64>::IsNull(void)
{
	return mX1==0.0 && mY1==0.0 && mX2==0.0 && mY2==0.0;
}

template<>
AWForceInline const awRegion<awF64>& awRegion<awF64>::SetNull(void)
{
	mX1 = mY1 = mX2 = mY2 = 0.0;
	return *this;
}

} }//namespace zen { namespace awtype {

#endif //__LibCore_Api_Type_Region_inl__
