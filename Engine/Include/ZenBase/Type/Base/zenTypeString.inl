namespace zen { namespace zenType {

zenString::zenString()
: maChar(1)
{
	maChar[0] = 0;
}

zenString::zenString(const zenString& _zString)	
: maChar(_zString.maChar)
{
}

zenString::zenString(const char* _zString)
: maChar(_zString, zenUInt(strlen(_zString)+1))
{
}

zenString& zenString::operator=(const zenString& _zString)
{
	maChar = _zString.maChar;
	return *this;
}
	
zenString& zenString::operator=(const char* _zString)
{
	maChar.Copy(_zString, zenUInt(strlen(_zString)+1));
	return *this;
}

zenString& zenString::operator+=(const zenString& _zString)
{		
	return *this += (const char*)_zString;
}

zenString& zenString::operator+=(const char* _zString)
{
	zenUInt uLen			= static_cast<zenUInt>(strlen(_zString));
	zenUInt uOldCount	= maChar.Count();
	maChar.SetCount(uOldCount + uLen);
	zenMem::Copy(&maChar[uOldCount-1], _zString, uLen+1);
	return *this;
}
zenString::operator const char*() const 
{ 
	return maChar.First();
}

const char* zenString::Last(zenUInt index)const
{
	return &maChar[maChar.Count() >= index+2 ? maChar.Count()-2-index : 0];
}

}}  //namespace zen, Type 


