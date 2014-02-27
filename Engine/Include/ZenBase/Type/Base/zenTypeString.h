#pragma once
#ifndef __zenBase_Type_String_h__
#define __zenBase_Type_String_h__

namespace zen { namespace zenType {

//=================================================================================================
//! @class		zenString
//-------------------------------------------------------------------------------------------------
//! @brief		Very primitive implementation at the moment, will grow with need
//! @todo		Support datasharing with same strings, format, etc...
//=================================================================================================
class zenString
{
ZENClassDeclareNoParent(zenString)
public:
	inline 				zenString();
	inline 				zenString(const zenString& _zString);
	inline 				zenString(const char* _zString);

	inline zenString&	operator=(const zenString& _zString);
	inline zenString&	operator=(const char* _zString);
	inline zenString&	operator+=(const zenString& _zString);
	inline zenString&	operator+=(const char* _zString);
	inline 				operator const char*() const;
	
	inline const char*	Last(zenUInt index=0)const;

protected:
	zenArrayDynamic<char>	maChar;
};

}}  //namespace zen, Type 

#include "zenTypeString.inl"

#endif
