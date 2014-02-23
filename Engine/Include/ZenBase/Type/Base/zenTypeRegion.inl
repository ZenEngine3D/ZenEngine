#pragma once
#ifndef __zenBase_Type_Region_inl__
#define __zenBase_Type_Region_inl__

namespace zen { namespace zenType {

template<>
AWForceInline zenRegion<zenS8>::zenRegion(void)
: mX1(0)
, mY1(0)
, mX2(0)
, mY2(0)
{
}

template<>
AWForceInline zenRegion<zenU8>::zenRegion(void)
: mX1(0)
, mY1(0)
, mX2(0)
, mY2(0)
{
}

template<>
AWForceInline zenRegion<zenS16>::zenRegion(void)
: mX1(0)
, mY1(0)
, mX2(0)
, mY2(0)
{
}

template<>
AWForceInline zenRegion<zenU16>::zenRegion(void)
: mX1(0)
, mY1(0)
, mX2(0)
, mY2(0)
{
}

template<>
AWForceInline zenRegion<zenS32>::zenRegion(void)
: mX1(0)
, mY1(0)
, mX2(0)
, mY2(0)
{
}

template<>
AWForceInline zenRegion<zenU32>::zenRegion(void)
: mX1(0)
, mY1(0)
, mX2(0)
, mY2(0)
{
}

template<>
AWForceInline zenRegion<zenS64>::zenRegion(void)
: mX1(0)
, mY1(0)
, mX2(0)
, mY2(0)
{
}

template<>
AWForceInline zenRegion<zenU64>::zenRegion(void)
: mX1(0)
, mY1(0)
, mX2(0)
, mY2(0)
{
}

template<>
AWForceInline zenRegion<zenF32>::zenRegion(void)
: mX1(0.0f)
, mY1(0.0f)
, mX2(0.0f)
, mY2(0.0f)
{
}

template<>
AWForceInline zenRegion<zenF64>::zenRegion(void)
: mX1(0.0f)
, mY1(0.0f)
, mX2(0.0f)
, mY2(0.0f)
{
}

template<class _Type_>
AWForceInline zenRegion<_Type_>::zenRegion(const _Type_ inX1, const _Type_ inY1, const _Type_ inX2, const _Type_ inY2)
: mX1(inX1)
, mY1(inY1)
, mX2(inX2)
, mY2(inY2)
{
}

template<class _Type_>
AWForceInline zenRegion<_Type_>::zenRegion(const zenRegion& xOther)
: mX1(xOther.mX1)
, mY1(xOther.mY1)
, mX2(xOther.mX2)
, mY2(xOther.mY2)
{
}

template<class _Type_>
AWForceInline bool zenRegion<_Type_>::operator!=(const zenRegion& xOther)
{
	return (mX1 != xOther.mX1 || mY1 != xOther.mY1 || mX2 != xOther.mX2 || mY2 != xOther.mY2);
}

template<class _Type_>
AWForceInline void zenRegion<_Type_>::Intersect(const zenRegion& xBox1, const zenRegion& xBox2)
{
	mX1 = std::max(xBox1.mX1, xBox2.mX1); mY1 = std::max(xBox1.mY1, xBox2.mY1);
	mX2 = std::min(xBox1.mX2, xBox2.mX2); mY2 = std::min(xBox1.mY2, xBox2.mY2);
}

template<class _Type_>
AWForceInline void zenRegion<_Type_>::Intersect(const zenRegion& xBox)
{
	mX1 = std::max(xBox.mX1, mX1); mY1 = std::max(xBox.mY1, mY1);
	mX2 = std::min(xBox.mX2, mX2); mY2 = std::min(xBox.mY2, mY2);
}

template<class _Type_>
AWForceInline bool zenRegion<_Type_>::IsInside(_Type_ inX, _Type_ inY)
{
	return (mX1 <= inX && mX2 >= inX && mY1 <= inY && mY2 >= inY);
}

template<class _Type_>
AWForceInline void zenRegion<_Type_>::Offset(const zenVector2<_Type_>* pxOffset)
{
	mX1 += pxOffset->mX1; mX2 += pxOffset->mX1;
	mX1 += pxOffset->mY1; mX2 += pxOffset->mY1;
}

template<class _Type_>
AWForceInline bool zenRegion<_Type_>::IsInside(const zenVector2<_Type_>* pxPos)
{
	return (mX1 <= pxPos->mX1 && mX2 >= pxPos->mX1 && mY1 <= pxPos->mY1 && mY2 >= pxPos->mY1);
}

template<>
AWForceInline bool zenRegion<zenS8>::IsNull(void)
{
	return mX1==0 && mY1==0 && mX2==0 && mY2==0;
}

template<>
AWForceInline const zenRegion<zenS8>& zenRegion<zenS8>::SetNull(void)
{
	mX1 = mY1 = mX2 = mY2 = 0;
	return *this;
}

template<>
AWForceInline bool zenRegion<zenU8>::IsNull(void)
{
	return mX1==0 && mY1==0 && mX2==0 && mY2==0;
}

template<>
AWForceInline const zenRegion<zenU8>& zenRegion<zenU8>::SetNull(void)
{
	mX1 = mY1 = mX2 = mY2 = 0;
	return *this;
}

template<>
AWForceInline bool zenRegion<zenS16>::IsNull(void)
{
	return mX1==0 && mY1==0 && mX2==0 && mY2==0;
}

template<>
AWForceInline const zenRegion<zenS16>& zenRegion<zenS16>::SetNull(void)
{
	mX1 = mY1 = mX2 = mY2 = 0;
	return *this;
}

template<>
AWForceInline bool zenRegion<zenU16>::IsNull(void)
{
	return mX1==0 && mY1==0 && mX2==0 && mY2==0;
}

template<>
AWForceInline const zenRegion<zenU16>& zenRegion<zenU16>::SetNull(void)
{
	mX1 = mY1 = mX2 = mY2 = 0;
	return *this;
}

template<>
AWForceInline bool zenRegion<zenS32>::IsNull(void)
{
	return mX1==0 && mY1==0 && mX2==0 && mY2==0;
}

template<>
AWForceInline const zenRegion<zenS32>& zenRegion<zenS32>::SetNull(void)
{
	mX1 = mY1 = mX2 = mY2 = 0;
	return *this;
}

template<>
AWForceInline bool zenRegion<zenU32>::IsNull(void)
{
	return mX1==0 && mY1==0 && mX2==0 && mY2==0;
}

template<>
AWForceInline const zenRegion<zenU32>& zenRegion<zenU32>::SetNull(void)
{
	mX1 = mY1 = mX2 = mY2 = 0;
	return *this;
}

template<>
AWForceInline bool zenRegion<zenS64>::IsNull(void)
{
	return mX1==0 && mY1==0 && mX2==0 && mY2==0;
}

template<>
AWForceInline const zenRegion<zenS64>& zenRegion<zenS64>::SetNull(void)
{
	mX1 = mY1 = mX2 = mY2 = 0;
	return *this;
}

template<>
AWForceInline bool zenRegion<zenU64>::IsNull(void)
{
	return mX1==0 && mY1==0 && mX2==0 && mY2==0;
}

template<>
AWForceInline const zenRegion<zenU64>& zenRegion<zenU64>::SetNull(void)
{
	mX1 = mY1 = mX2 = mY2 = 0;
	return *this;
}

template<>
AWForceInline bool zenRegion<zenF32>::IsNull(void)
{
	return mX1==0.0f && mY1==0.0f && mX2==0.0f && mY2==0.0f;
}

template<>
AWForceInline const zenRegion<zenF32>& zenRegion<zenF32>::SetNull(void)
{
	mX1 = mY1 = mX2 = mY2 = 0.0f;
	return *this;
}

template<>
AWForceInline bool zenRegion<zenF64>::IsNull(void)
{
	return mX1==0.0 && mY1==0.0 && mX2==0.0 && mY2==0.0;
}

template<>
AWForceInline const zenRegion<zenF64>& zenRegion<zenF64>::SetNull(void)
{
	mX1 = mY1 = mX2 = mY2 = 0.0;
	return *this;
}

} }//namespace zen { namespace zenType {

#endif //__zenBase_Type_Region_inl__
