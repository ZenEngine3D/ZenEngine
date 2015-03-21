#pragma once
#ifndef __zenApi_Base_Type_DebugString_h__
#define __zenApi_Base_Type_DebugString_h__

namespace zen { namespace zenType {

//=================================================================================================
//! @brief		Used to store string in Debug version only
//! @details	Stripped in release version
//=================================================================================================
class zDebugString
{
public:
	ZENInline			zDebugString();
	ZENInline			zDebugString(const char* _zString);
	ZENInline			operator const char*();
	ZENInline void		operator=(const zDebugString& _Copy);

protected:	
	ZENDbgCode(const char* mzDebugString;)
};
  

} } //namespace zen, Type

#include "zenBaseTypeDebugString.inl"

#endif
