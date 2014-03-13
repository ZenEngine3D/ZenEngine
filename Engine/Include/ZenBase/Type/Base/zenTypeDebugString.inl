namespace zen { namespace zenType {

zDebugString::zDebugString()
{		
	ZENDbgCode(mzDebugString="";)
}

zDebugString::zDebugString(const char* _zString)
{	
	ZENDbgCode(mzDebugString=_zString;)
}

zDebugString::operator const char*()
{
	ZENDbgCode(return mzDebugString;)
	return "";
}

void zDebugString::operator=(const zDebugString& _Copy)
{
	ZENDbgCode(mzDebugString = _Copy.mzDebugString;)
}

} } //namespace zen, Type
