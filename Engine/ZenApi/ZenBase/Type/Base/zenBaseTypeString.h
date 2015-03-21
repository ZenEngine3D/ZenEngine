#pragma once
#ifndef __zenApi_Base_Type_String_h__
#define __zenApi_Base_Type_String_h__

namespace zen { namespace zenType 
{

//=================================================================================================
//! @class		zString
//-------------------------------------------------------------------------------------------------
//! @brief		Very primitive implementation at the moment, will grow with need
//! @todo Optim: Support datasharing with same strings, format, etc...
//=================================================================================================
class zString 
{
ZENClassDeclareNoParent(zString)
public:
	ZENInline 				zString();
	ZENInline 				zString(const zString& _zString);
	ZENInline 				zString(const char* _zString);

	ZENInline zString&		operator=(const zString& _zString);
	ZENInline zString&		operator=(const char* _zString);
	ZENInline zString&		operator+=(const zString& _zString);
	ZENInline zString&		operator+=(const char* _zString);
	ZENInline 				operator const char*() const;
	ZENInline bool			operator==(const zString& _zString)const;
	ZENInline bool			operator==(const char* _zString)const;	
	ZENInline bool			operator!=(const zString& _zString)const;
	ZENInline bool			operator!=(const char* _zString)const;

	ZENInline const char*	Last(zUInt index=0)const;
	ZENInline void			Split(char _Separator, zArrayStatic<zString>& _aStringOut, zUInt _uAdditionalArraySize=0 )const;
	ZENInline void			Merge(const zArrayStatic<zString>& _aString, char _Separator, zInt _iMaxEntry=0);
	ZENInline zUInt			Len()const;
	zString&				Copy(const char* _zString, zUInt _uCount=0xFFFFFFFF);

	static void				Split(const char* _zString, char _Separator, zArrayStatic<zString>& _aStringOut, zUInt _uAdditionalArraySize=0 );
	static void				Merge(const zArrayStatic<zString>& _aStrings, char _Separator, zString& _zStringOut, zInt _iMaxEntry=0);
protected:
	zArrayDynamic<char>		maChar;
};

template<class THashType>
struct zStringHash
{ 		
	ZENInline zStringHash(const char* _zString="");				
	ZENInline const zStringHash& operator=(const zStringHash& _Copy);
	ZENInline bool operator==(const zStringHash& _Compare)const;
	ZENInline bool operator!=(const zStringHash& _Compare)const;
	ZENInline bool operator==(const THashType& _hCompare)const;
	ZENInline bool operator!=(const THashType& _hCompare)const;
	zString		mzName; 
	THashType	mhName; 

	static zUInt Find( THashType _HashName, const zStringHash* _pFirst, zUInt _uCount );
};

typedef zStringHash<zHash32> zStringHash32;
typedef zStringHash<zHash64> zStringHash64;	

}}  //namespace zen, Type 

#include "zenBaseTypeString.inl"

#endif
