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
	inline			zenDebugString();
	inline			zenDebugString(const char* _zString);
	inline			operator const char*();
	inline void		operator=(const zenDebugString& _Copy);

protected:	
	AWDbgCode(const char* mzDebugString;)
};
  

} } //namespace zen, Type

#include "zenTypeDebugString.inl"

#endif
