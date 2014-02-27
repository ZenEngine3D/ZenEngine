#pragma once
#ifndef __zenBase_Type_Defines_h__
#define __zenBase_Type_Defines_h__

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
#include "zenConstMacro.inl"

#define ZENArrayCount(_Array_)						(sizeof(_Array_)/sizeof((_Array_)[0]))
#define ZENMemberOffset(_Class_, _MemberName_)		( (zenUInt) &(((_Class_*)0)->_MemberName_) )
#define ZENMemberOffsetAfter(_Class_, _MemberName_)	( (zenUInt) &(((_Class_*)0)->_MemberName_) + sizeof((((_Class_*)0)->_MemberName_)) )

#define ZENClassDeclareNoParent(_Class_)			public: void zenClassDeclareCheck(){ ZENStaticAssertMsg( sizeof(*this) == sizeof(_Class_), "Wrong Class name in ClassDeclare macro" );}
#define ZENClassDeclare(_Class_, _ClassParent_)		ZENClassDeclareNoParent(_Class_)			\
													private: typedef _ClassParent_ Super;

//=================================================================================================
// Useful compile-time infos on type/class
//=================================================================================================
//! @brief Template magic determining if parameter is a pointer (at compile time)
template<typename T> struct ZenIsPointer					{ enum {value = false}; };	
template<typename T> struct ZenIsPointer<T*>				{ enum {value = true};	};	//!< @brief Template specialization detecting pointers

//! @brief	Template magic to determining if parameter type/class supports memcopy
//! @detail We can safely do memcopy on builtin type and pointers without problem.
//!			Classes that contains more than builtin type (int, float, ...) are not able to
//!			use memcopy for assignment, since their operator= might be doing some more complex work.
//!			If a class is safe, it should use the macro ZENSupportMemcopy to make the compiler know.
template<typename T> struct ZenSupportsMemCopy			{ enum {value = false}; };	
template<typename T> struct ZenSupportsMemCopy<T*>		{ enum {value = true};	};
template<typename T> struct ZenSupportsMemCopy<const T*>{ enum {value = true};	};
#define ZENSupportMemcopy(_CLASS_)	template<> struct ZenSupportsMemCopy<_CLASS_>	{ enum {value = true};	};


#include ZENHeaderPlatform(zenConstMacro)

#endif
