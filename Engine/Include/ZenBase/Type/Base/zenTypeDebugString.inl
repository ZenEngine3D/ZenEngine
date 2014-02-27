namespace zen { namespace zenType {

zenDebugString::zenDebugString()
{		
	ZENDbgCode(mzDebugString="";)
}

zenDebugString::zenDebugString(const char* _zString)
{	
	ZENDbgCode(mzDebugString=_zString;)
}

zenDebugString::operator const char*()
{
	ZENDbgCode(return mzDebugString;)
	return "";
}

void zenDebugString::operator=(const zenDebugString& _Copy)
{
	ZENDbgCode(mzDebugString = _Copy.mzDebugString;)
}

} } //namespace zen, Type
