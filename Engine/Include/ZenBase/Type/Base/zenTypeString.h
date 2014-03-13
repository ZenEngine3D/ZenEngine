#pragma once
#ifndef __zenBase_Type_String_h__
#define __zenBase_Type_String_h__

namespace zen { namespace zenType {

//=================================================================================================
//! @class		zString
//-------------------------------------------------------------------------------------------------
//! @brief		Very primitive implementation at the moment, will grow with need
//! @todo		Support datasharing with same strings, format, etc...
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
	
	ZENInline const char*	Last(zUInt index=0)const;

protected:
	zArrayDynamic<char>	maChar;
};

}}  //namespace zen, Type 

#include "zenTypeString.inl"

#endif
