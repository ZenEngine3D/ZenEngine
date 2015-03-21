namespace zen { namespace zenAss 
{

zAssetID::zAssetID() 
: meType(zenConst::keAssType__Invalid)
, muIndex(0)
{
}

zAssetID::zAssetID(zenConst::eAssetType _eType, zU32 _uIndex) 
: meType(_eType), muIndex(_uIndex)
{
}

zAssetID::zAssetID(const zAssetID& _Copy) 
: meType(_Copy.meType)
, muIndex(_Copy.muIndex)
{
}

zAssetID& zAssetID::operator=(const zAssetID& _Copy)
{ 
	meType = _Copy.meType; 
	muIndex = _Copy.muIndex; 
	return *this; 
}

bool zAssetID::operator==(const zAssetID& _Copy)const 
{ 
	return this->ToUInt() == _Copy.ToUInt();
}

bool zAssetID::operator!=(const zAssetID& _Copy)const
{
	return !(*this == _Copy);
}

bool zAssetID::IsValid()const
{ 
	return meType != zenConst::keAssType__Invalid; 
}

zU64 zAssetID::ToUInt()const
{
	return *reinterpret_cast<const zU64*>(this);
}

}} //namespace zen { namespace zenAss

