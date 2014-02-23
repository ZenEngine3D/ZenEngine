namespace zen { namespace awtype {

#if AW_DEBUGINFOON
static const char* szNullString = "";
#endif

awDebugString::awDebugString()
#if AW_DEBUGINFOON
: mzDebugString(szNullString)
#endif
{		
}

awDebugString::awDebugString(const char* _zString)
#if AW_DEBUGINFOON
: mzDebugString(_zString)
#endif
{		
}

awDebugString::operator const char*()
{
#if AW_DEBUGINFOON
	return mzDebugString;
#else
	return "";
#endif
}

void awDebugString::operator=(const awDebugString& _Copy)
{
#if AW_DEBUGINFOON
	mzDebugString = _Copy.mzDebugString;
#endif
}

} } //namespace zen, Type
