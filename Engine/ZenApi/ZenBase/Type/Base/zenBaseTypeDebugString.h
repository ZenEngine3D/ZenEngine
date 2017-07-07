#pragma once

namespace zen { namespace zenType {

//=================================================================================================
//! @brief		Used to store string in Debug version only
//! @details	Stripped in release version
//=================================================================================================
class zDebugString
{
public:
	zenInline			zDebugString();
	zenInline			zDebugString(const char* _zString);
	zenInline			operator const char*();
	zenInline void		operator=(const zDebugString& _Copy);

protected:	
	zenDbgCode(const char* mzDebugString;)
};
  

} } //namespace zen, Type

#include "zenBaseTypeDebugString.inl"

