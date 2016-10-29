#pragma once
#ifndef __zenBase_Type_Region_inl__
#define __zenBase_Type_Region_inl__

namespace zen { namespace zenType {

template<>
zenInlineForce zRegion<zI8>::zRegion(void)
: mX1(0)
, mY1(0)
, mX2(0)
, mY2(0)
{
}

template<>
zenInlineForce zRegion<zU8>::zRegion(void)
: mX1(0)
, mY1(0)
, mX2(0)
, mY2(0)
{
}

template<>
zenInlineForce zRegion<zI16>::zRegion(void)
: mX1(0)
, mY1(0)
, mX2(0)
, mY2(0)
{
}

template<>
zenInlineForce zRegion<zU16>::zRegion(void)
: mX1(0)
, mY1(0)
, mX2(0)
, mY2(0)
{
}

template<>
zenInlineForce zRegion<zI32>::zRegion(void)
: mX1(0)
, mY1(0)
, mX2(0)
, mY2(0)
{
}

template<>
zenInlineForce zRegion<zU32>::zRegion(void)
: mX1(0)
, mY1(0)
, mX2(0)
, mY2(0)
{
}

template<>
zenInlineForce zRegion<zI64>::zRegion(void)
: mX1(0)
, mY1(0)
, mX2(0)
, mY2(0)
{
}

template<>
zenInlineForce zRegion<zU64>::zRegion(void)
: mX1(0)
, mY1(0)
, mX2(0)
, mY2(0)
{
}

template<>
zenInlineForce zRegion<float>::zRegion(void)
: mX1(0.0f)
, mY1(0.0f)
, mX2(0.0f)
, mY2(0.0f)
{
}

template<>
zenInlineForce zRegion<double>::zRegion(void)
: mX1(0.0f)
, mY1(0.0f)
, mX2(0.0f)
, mY2(0.0f)
{
}

template<class _Type_>
zenInlineForce zRegion<_Type_>::zRegion(const _Type_ inX1, const _Type_ inY1, const _Type_ inX2, const _Type_ inY2)
: mX1(inX1)
, mY1(inY1)
, mX2(inX2)
, mY2(inY2)
{
}

template<class _Type_>
zenInlineForce zRegion<_Type_>::zRegion(const zRegion& xOther)
: mX1(xOther.mX1)
, mY1(xOther.mY1)
, mX2(xOther.mX2)
, mY2(xOther.mY2)
{
}

template<class _Type_>
zenInlineForce bool zRegion<_Type_>::operator!=(const zRegion& xOther)
{
	return (mX1 != xOther.mX1 || mY1 != xOther.mY1 || mX2 != xOther.mX2 || mY2 != xOther.mY2);
}

template<class _Type_>
zenInlineForce void zRegion<_Type_>::Intersect(const zRegion& xBox1, const zRegion& xBox2)
{
	mX1 = std::max(xBox1.mX1, xBox2.mX1); mY1 = std::max(xBox1.mY1, xBox2.mY1);
	mX2 = std::min(xBox1.mX2, xBox2.mX2); mY2 = std::min(xBox1.mY2, xBox2.mY2);
}

template<class _Type_>
zenInlineForce void zRegion<_Type_>::Intersect(const zRegion& xBox)
{
	mX1 = std::max(xBox.mX1, mX1); mY1 = std::max(xBox.mY1, mY1);
	mX2 = std::min(xBox.mX2, mX2); mY2 = std::min(xBox.mY2, mY2);
}

template<class _Type_>
zenInlineForce bool zRegion<_Type_>::IsInside(_Type_ inX, _Type_ inY)
{
	return (mX1 <= inX && mX2 >= inX && mY1 <= inY && mY2 >= inY);
}

template<class _Type_>
zenInlineForce void zRegion<_Type_>::Offset(const zVector2<_Type_>* pxOffset)
{
	mX1 += pxOffset->mX1; mX2 += pxOffset->mX1;
	mX1 += pxOffset->mY1; mX2 += pxOffset->mY1;
}

template<class _Type_>
zenInlineForce bool zRegion<_Type_>::IsInside(const zVector2<_Type_>* pxPos)
{
	return (mX1 <= pxPos->mX1 && mX2 >= pxPos->mX1 && mY1 <= pxPos->mY1 && mY2 >= pxPos->mY1);
}

template<>
zenInlineForce bool zRegion<zI8>::IsZero(void)
{
	return mX1==0 && mY1==0 && mX2==0 && mY2==0;
}

template<>
zenInlineForce const zRegion<zI8>& zRegion<zI8>::SetZero(void)
{
	mX1 = mY1 = mX2 = mY2 = 0;
	return *this;
}

template<>
zenInlineForce bool zRegion<zU8>::IsZero(void)
{
	return mX1==0 && mY1==0 && mX2==0 && mY2==0;
}

template<>
zenInlineForce const zRegion<zU8>& zRegion<zU8>::SetZero(void)
{
	mX1 = mY1 = mX2 = mY2 = 0;
	return *this;
}

template<>
zenInlineForce bool zRegion<zI16>::IsZero(void)
{
	return mX1==0 && mY1==0 && mX2==0 && mY2==0;
}

template<>
zenInlineForce const zRegion<zI16>& zRegion<zI16>::SetZero(void)
{
	mX1 = mY1 = mX2 = mY2 = 0;
	return *this;
}

template<>
zenInlineForce bool zRegion<zU16>::IsZero(void)
{
	return mX1==0 && mY1==0 && mX2==0 && mY2==0;
}

template<>
zenInlineForce const zRegion<zU16>& zRegion<zU16>::SetZero(void)
{
	mX1 = mY1 = mX2 = mY2 = 0;
	return *this;
}

template<>
zenInlineForce bool zRegion<zI32>::IsZero(void)
{
	return mX1==0 && mY1==0 && mX2==0 && mY2==0;
}

template<>
zenInlineForce const zRegion<zI32>& zRegion<zI32>::SetZero(void)
{
	mX1 = mY1 = mX2 = mY2 = 0;
	return *this;
}

template<>
zenInlineForce bool zRegion<zU32>::IsZero(void)
{
	return mX1==0 && mY1==0 && mX2==0 && mY2==0;
}

template<>
zenInlineForce const zRegion<zU32>& zRegion<zU32>::SetZero(void)
{
	mX1 = mY1 = mX2 = mY2 = 0;
	return *this;
}

template<>
zenInlineForce bool zRegion<zI64>::IsZero(void)
{
	return mX1==0 && mY1==0 && mX2==0 && mY2==0;
}

template<>
zenInlineForce const zRegion<zI64>& zRegion<zI64>::SetZero(void)
{
	mX1 = mY1 = mX2 = mY2 = 0;
	return *this;
}

template<>
zenInlineForce bool zRegion<zU64>::IsZero(void)
{
	return mX1==0 && mY1==0 && mX2==0 && mY2==0;
}

template<>
zenInlineForce const zRegion<zU64>& zRegion<zU64>::SetZero(void)
{
	mX1 = mY1 = mX2 = mY2 = 0;
	return *this;
}

template<>
zenInlineForce bool zRegion<float>::IsZero(void)
{
	return mX1==0.0f && mY1==0.0f && mX2==0.0f && mY2==0.0f;
}

template<>
zenInlineForce const zRegion<float>& zRegion<float>::SetZero(void)
{
	mX1 = mY1 = mX2 = mY2 = 0.0f;
	return *this;
}

template<>
zenInlineForce bool zRegion<double>::IsZero(void)
{
	return mX1==0.0 && mY1==0.0 && mX2==0.0 && mY2==0.0;
}

template<>
zenInlineForce const zRegion<double>& zRegion<double>::SetZero(void)
{
	mX1 = mY1 = mX2 = mY2 = 0.0;
	return *this;
}

} }//namespace zen { namespace zenType {

#endif //__zenBase_Type_Region_inl__
