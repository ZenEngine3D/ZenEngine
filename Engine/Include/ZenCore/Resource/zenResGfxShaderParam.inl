namespace zen { namespace awres {	
	
awShaderParameter::awShaderParameter(awHash32 _hName, awconst::eShaderElementType _eType, awU16 _uVectorSize, awU16 _uArrayCount, awU16 _uSizeTotal)
: mhName(_hName)
, meType( static_cast<awU16>(_eType))
, muVectorSize(_uVectorSize)
, muArrayCount(_uArrayCount)
, muSizeTotal(_uSizeTotal)
{}	

template <class TType, awconst::eShaderElementType TShaderType>
awShaderParamVector1<TType,TShaderType>::awShaderParamVector1( awHash32 _hName, TType _Value) 
: ParameterBase(_hName, TShaderType, 1, 0, sizeof(mValue))
, mValue(_Value)
{}	

template <class TType, awconst::eShaderElementType TShaderType>
awShaderParamVector2<TType,TShaderType>::awShaderParamVector2( awHash32 _hName, TType _Value1, TType _Value2) 
: awShaderParameter(_hName, TShaderType, 2, 0, sizeof(mValue))
{
	mValue[0] = _Value1;
	mValue[1] = _Value2;
}	

template <class TType, awconst::eShaderElementType TShaderType>
awShaderParamVector2<TType,TShaderType>::awShaderParamVector2( awHash32 _hName, const TType _Values[2]) 
: awShaderParameter(_hName, TShaderType, 2, 0, sizeof(mValue))
{
	mValue[0] = _Values[0];
	mValue[1] = _Values[1];
}
template <class TType, awconst::eShaderElementType TShaderType>
awShaderParamVector2<TType,TShaderType>::awShaderParamVector2( awHash32 _hName, const awVector2<TType>& _vValue) 
: awShaderParameter(_hName, TShaderType, 2, 0, sizeof(mValue))
{
	mValue[0] = _vValue.array[0];
	mValue[1] = _vValue.array[1];
}

template <class TType, awconst::eShaderElementType TShaderType>
awShaderParamVector3<TType,TShaderType>::awShaderParamVector3( awHash32 _hName, TType _Value1, TType _Value2, TType _Value3) 
: awShaderParameter(_hName, TShaderType, 3, 0, sizeof(mValue))
{
	mValue[0] = _Value1;
	mValue[1] = _Value2;
	mValue[2] = _Value3;
}

template <class TType, awconst::eShaderElementType TShaderType>
awShaderParamVector3<TType,TShaderType>::awShaderParamVector3( awHash32 _hName, const TType _Values[3]) 
: awShaderParameter(_hName, TShaderType, 3, 0, sizeof(mValue))
{
	mValue[0] = _Values[0];
	mValue[1] = _Values[1];
	mValue[2] = _Values[2];
}

template <class TType, awconst::eShaderElementType TShaderType>
awShaderParamVector3<TType,TShaderType>::awShaderParamVector3( awHash32 _hName, const awVector3<TType>& _vValue) 
: awShaderParameter(_hName, TShaderType, 3, 0, sizeof(mValue))
{
	mValue[0] = _vValue.array[0];
	mValue[1] = _vValue.array[1];
	mValue[2] = _vValue.array[2];
}

template <class TType, awconst::eShaderElementType TShaderType>
awShaderParamVector4<TType,TShaderType>::awShaderParamVector4( awHash32 _hName, TType _Value1, TType _Value2, TType _Value3, TType _Value4) 
: awShaderParameter(_hName, TShaderType, 4, 0, sizeof(mValue))
{
	mValue[0] = _Value1;
	mValue[1] = _Value2;
	mValue[2] = _Value3;
	mValue[3] = _Value4;
}	
template <class TType, awconst::eShaderElementType TShaderType>
awShaderParamVector4<TType,TShaderType>::awShaderParamVector4( awHash32 _hName, const TType _Values[4]) 
: awShaderParameter(_hName, TShaderType, 4, 0, sizeof(mValue))
{
	mValue[0] = _Values[0];
	mValue[1] = _Values[1];
	mValue[2] = _Values[2];
	mValue[3] = _Values[3];
}

template <class TType, awconst::eShaderElementType TShaderType>
awShaderParamVector4<TType,TShaderType>::awShaderParamVector4( awHash32 _hName, const awVector4<TType>& _vValues) 
: awShaderParameter(_hName, TShaderType, 4, 0, sizeof(mValue))
{
	mValue[0] = _vValues.xyzw[0];
	mValue[1] = _vValues.xyzw[1];
	mValue[2] = _vValues.xyzw[2];
	mValue[3] = _vValues.xyzw[3];
}

awShaderTexture::awShaderTexture()
: mhTextureName("")
, mTextureID()
, mSamplerID()
{}

awShaderTexture::awShaderTexture(awHash32 _hTextureName, awResourceID _TextureID, awResourceID _SamplerID)
: mhTextureName(_hTextureName)
, mTextureID(_TextureID)	
, mSamplerID(_SamplerID)
{}

awShaderDefine::awShaderDefine()
: mzName("")
, mzValue("")
{
}

awShaderDefine::awShaderDefine(const char* _zName, const char* _zValue)
: mzName(_zName)
, mzValue(_zValue)
{
}

awShaderDefine::awShaderDefine(const awString& _zName, const awString& _zValue)
: mzName(_zName)
, mzValue(_zValue)
{
}

}} // namespace zen, awres



