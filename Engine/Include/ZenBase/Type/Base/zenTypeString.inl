namespace zen { namespace zenType {

zString::zString()
: maChar(1)
{
	maChar[0] = 0;
}

zString::zString(const zString& _zString)	
: maChar(_zString.maChar)
{
}

zString::zString(const char* _zString)
: maChar(_zString, zUInt(strlen(_zString)+1))
{
}

zString& zString::operator=(const zString& _zString)
{
	maChar = _zString.maChar;
	return *this;
}
	
zString& zString::operator=(const char* _zString)
{
	maChar.Copy(_zString, zUInt(strlen(_zString)+1));
	return *this;
}

zString& zString::operator+=(const zString& _zString)
{		
	return *this += (const char*)_zString;
}

zString& zString::operator+=(const char* _zString)
{
	zUInt uLen			= static_cast<zUInt>(strlen(_zString));
	zUInt uOldCount	= maChar.Count();
	maChar.SetCount(uOldCount + uLen);
	zenMem::Copy(&maChar[uOldCount-1], _zString, uLen+1);
	return *this;
}
zString::operator const char*() const 
{ 
	return maChar.First();
}

const char* zString::Last(zUInt index)const
{
	return &maChar[maChar.Count() >= index+2 ? maChar.Count()-2-index : 0];
}

}}  //namespace zen, Type 


