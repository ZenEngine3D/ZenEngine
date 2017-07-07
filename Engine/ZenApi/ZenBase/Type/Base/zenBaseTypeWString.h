#pragma once

#include <string>

#define zenT(_Text_)	L##_Text_
typedef wchar_t zChar;

namespace zen { namespace zenType {

//typedef std::wstring zWString;
//! @todo Clean: replace zString with zWString, implement data sharing
class zWString : public std::wstring
{
zenClassDeclare(zWString, std::wstring)
public:
	zenInline					zWString();
	zenInline					zWString(const zWString& _Copy);
	zenInline					zWString(const std::wstring& _Copy);
	zenInline					zWString(const wchar_t* _pCopy);
	zenInline const zWString&	operator=(const zWString& _Copy);
	zenInline const zWString&	operator=(const std::wstring& _Copy);
	zenInline const zWString&	operator=(const wchar_t* _pCopy);	
	zenInline bool				operator==(const zWString& _zCmp)const;
	zenInline bool				operator!=(const zWString& _zCmp)const;
	zenInline bool				operator==(const wchar_t* _zCmp)const;
	zenInline bool				operator!=(const wchar_t* _zCmp)const;
	zenInline 					operator const zChar*() const;
	zenInline const zWString&	Prepend(const zWString& _Copy);
	void						Replace(wchar_t* _zSource, wchar_t* _zReplace, zUInt _uPos=0);

};

}}  //namespace zen, Type 

#include "zenBaseTypeWString.inl"
