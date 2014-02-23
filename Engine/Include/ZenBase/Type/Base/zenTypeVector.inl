namespace zen { namespace zenType {
	

template<typename TType>
const zenVector2<TType> zenVector2<TType>::Zero(TType(0), TType(0));
template<typename TType>
const zenVector2<TType> zenVector2<TType>::One(TType(1), TType(1));

template<typename TType>
const zenVector3<TType> zenVector3<TType>::Zero(TType(0), TType(0), TType(0));
template<typename TType>
const zenVector3<TType> zenVector3<TType>::One(TType(1), TType(1), TType(1));

template<typename TType>
const zenVector4<TType> zenVector4<TType>::Zero(TType(0), TType(0), TType(0), TType(0));
template<typename TType>
const zenVector4<TType> zenVector4<TType>::One(TType(1), TType(1), TType(1), TType(1));
	
//=============================================================================
// Vector2 Class
//=============================================================================
template<typename TType>
zenVector2<TType>::zenVector2()
: x(0)
, y(0) 
{
}

template<typename TType>
zenVector2<TType>::zenVector2(TType _Val)
: x(_Val)
, y(_Val) 
{}

template<typename TType>
zenVector2<TType>::zenVector2(TType aX, TType aY )
: x(aX)
, y(aY) 
{}

template<typename TType>
zenVector2<TType>::zenVector2(const zenVector2& _vCopy)
: x(_vCopy.x)
, y(_vCopy.y) 
{}

template<typename TType>
zenVector2<TType>& zenVector2<TType>::operator=(const zenVector2& _vCopy)
{
	xy[0] = _vCopy.xy[0];
	xy[1] = _vCopy.xy[1];
	return *this;
}

template<typename TType>
zenVector2<TType>& zenVector2<TType>::operator=(TType _Val)
{
	xy[0] = xy[1] = _Val;
	return *this;
}

template<typename TType>
zenVector2<TType> zenVector2<TType>::operator*(TType _Val)const
{ 
	zenVector2 vNewVal(*this); 
	vNewVal.xy[0] *= _Val; 
	vNewVal.xy[1] *= _Val; 
	return vNewVal;
}

template<typename TType>
zenVector2<TType> zenVector2<TType>::operator+(const zenVector2& _vAdd)const
{ 
	zenVector2 vNewVal(_vAdd); 
	vNewVal.xy[0] += xy[0];
	vNewVal.xy[1] += xy[1];
	return vNewVal;
}

template<typename TType>
bool zenVector2<TType>::operator==(const zenVector2& _vCompare)const
{
	return x==_vCompare.x && y==_vCompare.y;
}

template<typename TType>
bool zenVector2<TType>::operator!=(const zenVector2& _vCompare)const
{
	return !(*this==_vCompare);
}

template<typename TType>
bool zenVector2<TType>::IsNull()
{
	return x==0 && y==0;
}

template<typename TType>
const zenVector2<TType>& zenVector2<TType>::SetNull()
{
	x = y = 0;
	return *this;
}

//=============================================================================
// Vector3 Class
//=============================================================================
template<typename TType>
zenVector3<TType>::zenVector3()
: x(0)
, y(0)
, z(0) 
{}

template<typename TType>
zenVector3<TType>::zenVector3(TType _val)
: x(_val)
, y(_val)
, z(_val) 
{}

template<typename TType>
zenVector3<TType>::zenVector3(TType aX, TType aY, TType aZ )
: x(aX)
, y(aY)
, z(aZ) 
{}

template<typename TType>
zenVector3<TType>::zenVector3(const zenVector3& _vCopy)
: x(_vCopy.x)
, y(_vCopy.y)
, z(_vCopy.z) 
{}

template<typename TType>
zenVector3<TType>& zenVector3<TType>::operator=(const zenVector3& _vCopy)
{
	xyz[0] = _vCopy.xyz[0];
	xyz[1] = _vCopy.xyz[1];
	xyz[2] = _vCopy.xyz[2];
	return *this;
}

template<typename TType>
zenVector3<TType>& zenVector3<TType>::operator=(TType _Val)
{
	xyz[0] = xyz[1] = xyz[2] =_Val;
	return *this;
}

template<typename TType>
zenVector3<TType> zenVector3<TType>::operator*(TType _Val)const
{ 
	zenVector3 vNewVal(*this); 
	vNewVal.xyz[0] *= _Val;
	vNewVal.xyz[1] *= _Val;
	vNewVal.xyz[2] *= _Val;
	return vNewVal;
}

template<typename TType>
zenVector3<TType> zenVector3<TType>::operator+(const zenVector3& _vAdd)const
{ 
	zenVector3 vNewVal(_vAdd); 
	vNewVal.xyz[0] += xyz[0];
	vNewVal.xyz[1] += xyz[1];
	vNewVal.xyz[2] += xyz[2];
	return vNewVal;
}

template<typename TType>
bool zenVector3<TType>::operator==(const zenVector3& _vCompare)const
{
	return x==_vCompare.x && y==_vCompare.y && z==_vCompare.z;
}

template<typename TType>
bool zenVector3<TType>::operator!=(const zenVector3& _vCompare)const
{
	return !(*this==_vCompare);
}

template<typename TType>
bool zenVector3<TType>::IsNull()
{
	return x==0 && y==0 && z==0;
}

template<typename TType>
const zenVector3<TType>& zenVector3<TType>::SetNull()
{
	x = y = z = 0;
	return *this;
}

//=============================================================================
// Vector4 Class
//=============================================================================
template<typename TType>
zenVector4<TType>::zenVector4()
: x(0)
, y(0)
, z(0)
, w(0) 
{}

template<typename TType>
zenVector4<TType>::zenVector4(TType _Val)
: x(_Val)
, y(_Val)
, z(_Val)
, w(_Val) 
{}

template<typename TType>
zenVector4<TType>::zenVector4(TType aX, TType aY, TType aZ, TType aW )
: x(aX)
, y(aY)
, z(aZ)
, w(aW) 
{}

template<typename TType>
zenVector4<TType>::zenVector4(const zenVector4& _vCopy)
: x(_vCopy.x)
, y(_vCopy.y)
, z(_vCopy.z)
, w(_vCopy.w) 
{}

template<typename TType>
zenVector4<TType>& zenVector4<TType>::operator=(const zenVector4& _vCopy)
{
	xyzw[0] = _vCopy.xyzw[0];
	xyzw[1] = _vCopy.xyzw[1];
	xyzw[2] = _vCopy.xyzw[2];
	xyzw[3] = _vCopy.xyzw[3];
	return *this;
}

template<typename TType>
zenVector4<TType>& zenVector4<TType>::operator=(TType _Val)
{
	xyzw[0] = xyzw[1] = xyzw[2] = xyzw[3] = _Val;
	return *this;
}

template<typename TType>
zenVector4<TType> zenVector4<TType>::operator*(TType _Val)const
{ 
	zenVector4 vNewVal(*this); 
	vNewVal.xyzw[0] *= _Val; 
	vNewVal.xyzw[1] *= _Val; 
	vNewVal.xyzw[2] *= _Val; 
	vNewVal.xyzw[3] *= _Val; 
	return vNewVal;
}

template<typename TType>
zenVector4<TType> zenVector4<TType>::operator+(const zenVector4& _vAdd)const
{ 
	zenVector4 vNewVal(_vAdd); 
	vNewVal.xyzw[0] += xyzw[0];
	vNewVal.xyzw[1] += xyzw[1];
	vNewVal.xyzw[2] += xyzw[2];
	vNewVal.xyzw[3] += xyzw[3];
	return vNewVal;
}

template<typename TType>
bool zenVector4<TType>::operator==(const zenVector4& _vCompare)const
{
	return x==_vCompare.x && y==_vCompare.y && z==_vCompare.z && w==_vCompare.w;
}

template<typename TType>
bool zenVector4<TType>::operator!=(const zenVector4& _vCompare)const
{
	return !(*this==_vCompare);
}

template<typename TType>
bool zenVector4<TType>::IsNull()
{
	return x==0 && y==0 && z==0 && w==0;
}

template<typename TType>
const zenVector4<TType>& zenVector4<TType>::SetNull()
{
	x = y = z = w = 0;
	return *this;
}

}} // namespace zen, zenType
