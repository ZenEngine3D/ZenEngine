#pragma once
#ifndef __LibCore_Api_Type_DebugString_h__
#define __LibCore_Api_Type_DebugString_h__

namespace zen { namespace awtype {

//=================================================================================================
//! @brief		Used to store string in Debug version only
//! @details	Stripped in release version
//=================================================================================================
class awDebugString
{
public:
	inline			awDebugString();
	inline			awDebugString(const char* _zString);
	inline			operator const char*();
	inline void		operator=(const awDebugString& _Copy);

protected:	
#if AW_DEBUGINFOON
	const char*		mzDebugString;
#endif
};
  

} } //namespace zen, Type

#include "zenTypeDebugString.inl"

#endif
