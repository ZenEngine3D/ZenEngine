#pragma once
#ifndef __zenApi_Base_Type_WString_h__
#define __zenApi_Base_Type_WString_h__

#include <string>

#define zenT(_Text_)	L##_Text_
typedef wchar_t zChar;

namespace zen { namespace zenType {

//typedef std::wstring zWString;
//! @todo Clean: replace zString with zWString, implement data sharing
class zWString : public std::wstring
{
ZENClassDeclare(zWString, std::wstring)
public:
	ZENInline					zWString();
	ZENInline					zWString(const zWString& _Copy);
	ZENInline					zWString(const std::wstring& _Copy);
	ZENInline					zWString(const wchar_t* _pCopy);
	ZENInline const zWString&	operator=(const zWString& _Copy);
	ZENInline const zWString&	operator=(const std::wstring& _Copy);
	ZENInline const zWString&	operator=(const wchar_t* _pCopy);	
	ZENInline bool				operator==(const zWString& _zCmp)const;
	ZENInline bool				operator!=(const zWString& _zCmp)const;
	ZENInline bool				operator==(const wchar_t* _zCmp)const;
	ZENInline bool				operator!=(const wchar_t* _zCmp)const;
	ZENInline 					operator const zChar*() const;
	ZENInline const zWString&	Prepend(const zWString& _Copy);
	void						Replace(wchar_t* _zSource, wchar_t* _zReplace, zUInt _uPos=0);

};

}}  //namespace zen, Type 

#include "zenBaseTypeWString.inl"

#endif
