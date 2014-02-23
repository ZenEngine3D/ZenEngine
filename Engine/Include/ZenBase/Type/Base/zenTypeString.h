#pragma once
#ifndef __LibCore_Api_Type_String_h__
#define __LibCore_Api_Type_String_h__

#include <zenBase/Memory/zenMemUtils.h>

namespace zen { namespace awtype {

//=================================================================================================
//! @class		awString
//-------------------------------------------------------------------------------------------------
//! @brief		Very primitive implementation at the moment, will grow with need
//! @todo		Support datasharing with same strings, format, etc...
//=================================================================================================
class awString
{
AWClassDeclareNoParent(awString)
public:
	inline 				awString();
	inline 				awString(const awString& _zString);
	inline 				awString(const char* _zString);

	inline awString&	operator=(const awString& _zString);
	inline awString&	operator=(const char* _zString);
	inline awString&	operator+=(const awString& _zString);
	inline awString&	operator+=(const char* _zString);
	inline 				operator const char*() const;
	
	inline const char*	Last(awUInt index=0)const;

protected:
	awArrayDynamic<char>	maChar;
};

}}  //namespace zen, Type 

#include "zenTypeString.inl"

#endif
