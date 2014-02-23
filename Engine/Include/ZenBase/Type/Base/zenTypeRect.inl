namespace zen { namespace zenType {

template<>
AWForceInline zenRect<zenS8>::zenRect(void)
: mX(0)
, mY(0)
, mW(0)
, mH(0)
{
}

template<>
AWForceInline zenRect<zenU8>::zenRect(void)
: mX(0)
, mY(0)
, mW(0)
, mH(0)
{
}

template<>
AWForceInline zenRect<zenS16>::zenRect(void)
: mX(0)
, mY(0)
, mW(0)
, mH(0)
{
}

template<>
AWForceInline zenRect<zenU16>::zenRect(void)
: mX(0)
, mY(0)
, mW(0)
, mH(0)
{
}

template<>
AWForceInline zenRect<zenS32>::zenRect(void)
: mX(0)
, mY(0)
, mW(0)
, mH(0)
{
}

template<>
AWForceInline zenRect<zenU32>::zenRect(void)
: mX(0)
, mY(0)
, mW(0)
, mH(0)
{
}

template<>
AWForceInline zenRect<zenS64>::zenRect(void)
: mX(0)
, mY(0)
, mW(0)
, mH(0)
{
}

template<>
AWForceInline zenRect<zenU64>::zenRect(void)
: mX(0)
, mY(0)
, mW(0)
, mH(0)
{
}

template<>
AWForceInline zenRect<zenF32>::zenRect(void)
: mX(0.0f)
, mY(0.0f)
, mW(0.0f)
, mH(0.0f)
{
}

template<>
AWForceInline zenRect<zenF64>::zenRect(void)
: mX(0.0f)
, mY(0.0f)
, mW(0.0f)
, mH(0.0f)
{
}

template<class _Type_>
AWForceInline zenRect<_Type_>::zenRect(const _Type_& inX, const _Type_& inY, const _Type_& inW, const _Type_& inH)
: mX(inX)
, mY(inY)
, mW(inW)
, mH(inH)
{
}

template<>
AWForceInline bool zenRect<zenS8>::IsNull(void)
{
	return mX==0 && mY==0 && mW==0 && mH==0;
}

template<>
AWForceInline const zenRect<zenS8>& zenRect<zenS8>::SetNull(void)
{
	mX = mY = mW = mH = 0;
	return *this;
}

template<>
AWForceInline bool zenRect<zenU8>::IsNull(void)
{
	return mX==0 && mY==0 && mW==0 && mH==0;
}

template<>
AWForceInline const zenRect<zenU8>& zenRect<zenU8>::SetNull(void)
{
	mX = mY = mW = mH = 0;
	return *this;
}

template<>
AWForceInline bool zenRect<zenS16>::IsNull(void)
{
	return mX==0 && mY==0 && mW==0 && mH==0;
}

template<>
AWForceInline const zenRect<zenS16>& zenRect<zenS16>::SetNull(void)
{
	mX = mY = mW = mH = 0;
	return *this;
}

template<>
AWForceInline bool zenRect<zenU16>::IsNull(void)
{
	return mX==0 && mY==0 && mW==0 && mH==0;
}

template<>
AWForceInline const zenRect<zenU16>& zenRect<zenU16>::SetNull(void)
{
	mX = mY = mW = mH = 0;
	return *this;
}

template<>
AWForceInline bool zenRect<zenS32>::IsNull(void)
{
	return mX==0 && mY==0 && mW==0 && mH==0;
}

template<>
AWForceInline const zenRect<zenS32>& zenRect<zenS32>::SetNull(void)
{
	mX = mY = mW = mH = 0;
	return *this;
}

template<>
AWForceInline bool zenRect<zenU32>::IsNull(void)
{
	return mX==0 && mY==0 && mW==0 && mH==0;
}

template<>
AWForceInline const zenRect<zenU32>& zenRect<zenU32>::SetNull(void)
{
	mX = mY = mW = mH = 0;
	return *this;
}

template<>
AWForceInline bool zenRect<zenS64>::IsNull(void)
{
	return mX==0 && mY==0 && mW==0 && mH==0;
}

template<>
AWForceInline const zenRect<zenS64>& zenRect<zenS64>::SetNull(void)
{
	mX = mY = mW = mH = 0;
	return *this;
}

template<>
AWForceInline bool zenRect<zenU64>::IsNull(void)
{
	return mX==0 && mY==0 && mW==0 && mH==0;
}

template<>
AWForceInline const zenRect<zenU64>& zenRect<zenU64>::SetNull(void)
{
	mX = mY = mW = mH = 0;
	return *this;
}

template<>
AWForceInline bool zenRect<zenF32>::IsNull(void)
{
	return mX==0.0f && mY==0.0f && mW==0.0f && mH==0.0f;
}

template<>
AWForceInline const zenRect<zenF32>& zenRect<zenF32>::SetNull(void)
{
	mX = mY = mW = mH = 0.0f;
	return *this;
}

template<>
AWForceInline bool zenRect<zenF64>::IsNull(void)
{
	return mX==0.0 && mY==0.0 && mW==0.0 && mH==0.0;
}

template<>
AWForceInline const zenRect<zenF64>& zenRect<zenF64>::SetNull(void)
{
	mX = mY = mW = mH = 0.0;
	return *this;
}

template<class _Type_>
AWForceInline zenRect<_Type_>::zenRect(const zenRect& xOther)
	: mX(xOther.mX)
	, mY(xOther.mY)
	, mW(xOther.mW)
	, mH(xOther.mH)
{
}

}}//namespace zen { namespace zenType {

