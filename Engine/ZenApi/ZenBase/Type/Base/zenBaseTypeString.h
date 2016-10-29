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
zenClassDeclareNoParent(zString)
public:
	zenInline 				zString();
	zenInline 				zString(const zString& _zString);
	zenInline 				zString(const char* _zString);

	zenInline zString&		operator=(const zString& _zString);
	zenInline zString&		operator=(const char* _zString);
	zenInline zString&		operator+=(const zString& _zString);
	zenInline zString&		operator+=(const char* _zString);
	zenInline 				operator const char*() const;
	zenInline bool			operator==(const zString& _zString)const;
	zenInline bool			operator==(const char* _zString)const;	
	zenInline bool			operator!=(const zString& _zString)const;
	zenInline bool			operator!=(const char* _zString)const;

	zenInline const char*	Last(zUInt index=0)const;
	zenInline void			Split(char _Separator, zArrayStatic<zString>& _aStringOut, zUInt _uAdditionalArraySize=0 )const;
	zenInline void			Merge(const zArrayStatic<zString>& _aString, char _Separator, zInt _iMaxEntry=0);
	zenInline zUInt			Len()const;
	zString&				Copy(const char* _zString, zUInt _uCount=0xFFFFFFFF);

	static void				Split(const char* _zString, char _Separator, zArrayStatic<zString>& _aStringOut, zUInt _uAdditionalArraySize=0 );
	static void				Merge(const zArrayStatic<zString>& _aStrings, char _Separator, zString& _zStringOut, zInt _iMaxEntry=0);
protected:
	zArrayDynamic<char>		maChar;
};

template<class THashType>
struct zStringHash
{ 		
	zenInline zStringHash(const char* _zString="");				
	zenInline const zStringHash& operator=(const zStringHash& _Copy);
	zenInline bool operator==(const zStringHash& _Compare)const;
	zenInline bool operator!=(const zStringHash& _Compare)const;
	zenInline bool operator==(const THashType& _hCompare)const;
	zenInline bool operator!=(const THashType& _hCompare)const;
	zString		mzName; 
	THashType	mhName; 
	static zUInt Find( THashType _HashName, const zStringHash* _pFirst, zUInt _uCount );
};

typedef zStringHash<zHash32> zStringHash32;
typedef zStringHash<zHash64> zStringHash64;	

}}  //namespace zen, Type 

#include "zenBaseTypeString.inl"

#endif
