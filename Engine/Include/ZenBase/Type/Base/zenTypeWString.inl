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

}}  //namespace zen, Type 


