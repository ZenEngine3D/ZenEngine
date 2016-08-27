#pragma once
#ifndef __zenApi_Base_Type_Defines_h__
#define __zenApi_Base_Type_Defines_h__

//=================================================================================================
//! @file		CTypDefines.h
//! @brief		Various defines constants and macros
//! @details	Activate various feature based on built-type, 
//!				and add some helpful macros. 
//!				For constant type defines, look into @link CTypConstants.h @endlink
//=================================================================================================

//! @cond DOXYGEN_IGNORE
#define EMPTY																								//Empty define
#define ZENEmpty1(_A)																						//Empty define with 1 parameter
#define ZENEmpty2(_A,_B)																					// ...
#define ZENEmpty3(_A,_B,_C)																		
#define ZENEmpty4(_A,_B,_C,_D)																	
#define ZENEmpty5(_A,_B,_C,_D,_E)																

#define ZENDefineStich2B(DEFINE1, DEFINE2)			DEFINE1##DEFINE2
#define ZENDefineStich3B(DEFINE1, DEFINE2, DEFINE3)	DEFINE1##DEFINE2##DEFINE3
#define ZENDefineStich2(DEFINE1, DEFINE2)			ZENDefineStich2B(DEFINE1,DEFINE2)
#define ZENDefineStich3(DEFINE1, DEFINE2, DEFINE3)	ZENDefineStich3B(DEFINE1,DEFINE2,DEFINE3)
//! @endcond DOXYGEN_IGNORE


//=================================================================================================
// Various support macros
//=================================================================================================
#include "zenBaseConstMacro.inl"

#define zenArrayCount(_Array_)						(sizeof(_Array_)/sizeof((_Array_)[0]))
#define zenClassDeclareNoParent(_Class_)			public: void zenClassDeclareCheck(){ zenStaticAssertMsg( sizeof(*this) == sizeof(_Class_), "Wrong Class name in ClassDeclare macro" );}
#define zenClassDeclare(_Class_, _ClassParent_)		zenClassDeclareNoParent(_Class_)			\
													public: typedef _ClassParent_ Super;
#include ZENHeaderPlatform(zenBaseConstMacro)

#endif

