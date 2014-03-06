#pragma once
#ifndef __zenBase_Type_DebugString_h__
#define __zenBase_Type_DebugString_h__

namespace zen { namespace zenType {

//=================================================================================================
//! @brief		Used to store string in Debug version only
//! @details	Stripped in release version
//=================================================================================================
class zenDebugString
{
public:
	ZENInline			zenDebugString();
	ZENInline			zenDebugString(const char* _zString);
	ZENInline			operator const char*();
	ZENInline void		operator=(const zenDebugString& _Copy);

protected:	
	ZENDbgCode(const char* mzDebugString;)
};
  

} } //namespace zen, Type

#include "zenTypeDebugString.inl"

#endif
