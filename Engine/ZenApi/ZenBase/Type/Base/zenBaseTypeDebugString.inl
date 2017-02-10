namespace zen { namespace zenType {

zDebugString::zDebugString()
{		
	zenDbgCode(mzDebugString="";)
}

zDebugString::zDebugString(const char* _zString)
{	
	zenDbgCode(mzDebugString=_zString;)
}

zDebugString::operator const char*()
{
	zenDbgCode(return mzDebugString;)
	return "";
}

void zDebugString::operator=(const zDebugString& _Copy)
{
	zenDbgCode(mzDebugString = _Copy.mzDebugString;)
}

} } //namespace zen, Type
