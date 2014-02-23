namespace zen { namespace zenType {

zenDebugString::zenDebugString()
{		
	AWDbgCode(mzDebugString="";)
}

zenDebugString::zenDebugString(const char* _zString)
{	
	AWDbgCode(mzDebugString=_zString;)
}

zenDebugString::operator const char*()
{
	AWDbgCode(return mzDebugString;)
	return "";
}

void zenDebugString::operator=(const zenDebugString& _Copy)
{
	AWDbgCode(mzDebugString = _Copy.mzDebugString;)
}

} } //namespace zen, Type
