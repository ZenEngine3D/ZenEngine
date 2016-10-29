namespace zen { namespace zenRes {	
	
zShaderParameter::zShaderParameter(zHash32 _hName, zenConst::eShaderElementType _eType, zU16 _uVectorSize, zU16 _uArrayCount, zU16 _uSizeTotal)
: mhName(_hName)
, meType( static_cast<zU16>(_eType))
, muVectorSize(_uVectorSize)
, muArrayCount(_uArrayCount)
, muSizeTotal(_uSizeTotal)
{}	

template <class TType, zenConst::eShaderElementType TShaderType>
zShaderParamVector1<TType,TShaderType>::zShaderParamVector1( zHash32 _hName, TType _Value) 
: ParameterBase(_hName, TShaderType, 1, 0, sizeof(mValue))
, mValue(_Value)
{}	

template <class TType, zenConst::eShaderElementType TShaderType>
zShaderParamVector2<TType,TShaderType>::zShaderParamVector2( zHash32 _hName, TType _Value1, TType _Value2) 
: zShaderParameter(_hName, TShaderType, 2, 0, sizeof(mValue))
{
	mValue[0] = _Value1;
	mValue[1] = _Value2;
}	

template <class TType, zenConst::eShaderElementType TShaderType>
zShaderParamVector2<TType,TShaderType>::zShaderParamVector2( zHash32 _hName, const TType _Values[2]) 
: zShaderParameter(_hName, TShaderType, 2, 0, sizeof(mValue))
{
	mValue[0] = _Values[0];
	mValue[1] = _Values[1];
}
template <class TType, zenConst::eShaderElementType TShaderType>
zShaderParamVector2<TType,TShaderType>::zShaderParamVector2( zHash32 _hName, const zVector2<TType>& _vValue) 
: zShaderParameter(_hName, TShaderType, 2, 0, sizeof(mValue))
{
	mValue[0] = _vValue.array[0];
	mValue[1] = _vValue.array[1];
}

template <class TType, zenConst::eShaderElementType TShaderType>
zShaderParamVector3<TType,TShaderType>::zShaderParamVector3( zHash32 _hName, TType _Value1, TType _Value2, TType _Value3) 
: zShaderParameter(_hName, TShaderType, 3, 0, sizeof(mValue))
{
	mValue[0] = _Value1;
	mValue[1] = _Value2;
	mValue[2] = _Value3;
}

template <class TType, zenConst::eShaderElementType TShaderType>
zShaderParamVector3<TType,TShaderType>::zShaderParamVector3( zHash32 _hName, const TType _Values[3]) 
: zShaderParameter(_hName, TShaderType, 3, 0, sizeof(mValue))
{
	mValue[0] = _Values[0];
	mValue[1] = _Values[1];
	mValue[2] = _Values[2];
}

template <class TType, zenConst::eShaderElementType TShaderType>
zShaderParamVector3<TType,TShaderType>::zShaderParamVector3( zHash32 _hName, const zVector3<TType>& _vValue) 
: zShaderParameter(_hName, TShaderType, 3, 0, sizeof(mValue))
{
	mValue[0] = _vValue.array[0];
	mValue[1] = _vValue.array[1];
	mValue[2] = _vValue.array[2];
}

template <class TType, zenConst::eShaderElementType TShaderType>
zShaderParamVector4<TType,TShaderType>::zShaderParamVector4( zHash32 _hName, TType _Value1, TType _Value2, TType _Value3, TType _Value4) 
: zShaderParameter(_hName, TShaderType, 4, 0, sizeof(mValue))
{
	mValue[0] = _Value1;
	mValue[1] = _Value2;
	mValue[2] = _Value3;
	mValue[3] = _Value4;
}	
template <class TType, zenConst::eShaderElementType TShaderType>
zShaderParamVector4<TType,TShaderType>::zShaderParamVector4( zHash32 _hName, const TType _Values[4]) 
: zShaderParameter(_hName, TShaderType, 4, 0, sizeof(mValue))
{
	mValue[0] = _Values[0];
	mValue[1] = _Values[1];
	mValue[2] = _Values[2];
	mValue[3] = _Values[3];
}

template <class TType, zenConst::eShaderElementType TShaderType>
zShaderParamVector4<TType,TShaderType>::zShaderParamVector4( zHash32 _hName, const zVector4<TType>& _vValues) 
: zShaderParameter(_hName, TShaderType, 4, 0, sizeof(mValue))
{
	mValue[0] = _vValues.xyzw[0];
	mValue[1] = _vValues.xyzw[1];
	mValue[2] = _vValues.xyzw[2];
	mValue[3] = _vValues.xyzw[3];
}

zShaderResource::zShaderResource()
: mhResourceName("")
, mResourceID()
{}

zShaderResource::zShaderResource(zHash32 _hTextureName, zResID _ResourceID)
: mhResourceName(_hTextureName)
, mResourceID(_ResourceID)	
{}

zShaderDefine::zShaderDefine()
: mzName("")
, mzValue("")
{
}

zShaderDefine::zShaderDefine(const char* _zName, const char* _zValue)
: mzName(_zName)
, mzValue(_zValue)
{
}

zShaderDefine::zShaderDefine(const zString& _zName, const zString& _zValue)
: mzName(_zName)
, mzValue(_zValue)
{
}

}} // namespace zen, zenRes



