namespace zen { namespace zenType {

zWString::zWString()
: Super()
{
}

zWString::zWString(const zWString& _Copy)
	: Super(_Copy)
{
}

zWString::zWString(const std::wstring& _Copy)
	: Super(_Copy)
{
}

zWString::zWString(const wchar_t* _pCopy)
	: Super(_pCopy)
{
}

const zWString& zWString::operator=(const zWString& _Copy)
{
	Super::operator=(_Copy);
	return *this;
}

const zWString& zWString::operator=(const std::wstring& _Copy)
{
	Super::operator=(_Copy);
	return *this;
}

const zWString& zWString::operator=(const wchar_t* _pCopy)
{
	Super::operator=(_pCopy);
	return *this;
}

zWString::operator const zChar*() const
{
	return c_str();
}

bool zWString::operator==(const zWString& _zCmp)const
{
	return compare( _zCmp ) == 0;
}

bool zWString::operator!=(const zWString& _zCmp)const
{
	return !(*this == _zCmp);
}

bool zWString::operator==(const wchar_t* _zCmp)const
{
	return compare(_zCmp) == 0;
}

bool zWString::operator!=(const wchar_t* _zCmp)const
{
	return !(*this == _zCmp);
}

const zWString&	zWString::Prepend(const zWString& _Copy)
{
	this->insert(0, _Copy);
	return *this;
}


}}  //namespace zen, Type 


