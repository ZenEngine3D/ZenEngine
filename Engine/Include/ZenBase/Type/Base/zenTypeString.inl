namespace zen { namespace awtype {

awString::awString()
: maChar(1)
{
	maChar[0] = 0;
}

awString::awString(const awString& _zString)	
: maChar(_zString.maChar)
{
}

awString::awString(const char* _zString)
: maChar(_zString, awUInt(strlen(_zString)+1))
{
}

awString& awString::operator=(const awString& _zString)
{
	maChar = _zString.maChar;
	return *this;
}
	
awString& awString::operator=(const char* _zString)
{
	maChar.Copy(_zString, awUInt(strlen(_zString)+1));
	return *this;
}

awString& awString::operator+=(const awString& _zString)
{		
	return *this += (const char*)_zString;
}

awString& awString::operator+=(const char* _zString)
{
	awUInt uLen			= static_cast<awUInt>(strlen(_zString));
	awUInt uOldCount	= maChar.Count();
	maChar.SetCount(uOldCount + uLen);
	zenMem::Copy(&maChar[uOldCount-1], _zString, uLen+1);
	return *this;
}
awString::operator const char*() const 
{ 
	return maChar.First();
}

const char* awString::Last(awUInt index)const
{
	return &maChar[maChar.Count() >= index+2 ? maChar.Count()-2-index : 0];
}

}}  //namespace zen, Type 


