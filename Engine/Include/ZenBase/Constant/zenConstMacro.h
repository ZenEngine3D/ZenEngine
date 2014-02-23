#pragma once
#ifndef __LibCore_Type_Defines_h__
#define __LibCore_Type_Defines_h__

//=================================================================================================
//! @file		CTypDefines.h
//! @brief		Various defines constants and macros
//! @details	Activate various feature based on built-type, 
//!				and add some helpful macros. 
//!				For constant type defines, look into @link CTypConstants.h @endlink
//=================================================================================================

//! @cond DOXYGEN_IGNORE
#define EMPTY																								//Empty define
#define AWEmpty1(_A)																						//Empty define with 1 parameter
#define AWEmpty2(_A,_B)																						// ...
#define AWEmpty3(_A,_B,_C)																		
#define AWEmpty4(_A,_B,_C,_D)																	
#define AWEmpty5(_A,_B,_C,_D,_E)																

#define AWDefineStich2B(DEFINE1, DEFINE2)			DEFINE1##DEFINE2
#define AWDefineStich3B(DEFINE1, DEFINE2, DEFINE3)	DEFINE1##DEFINE2##DEFINE3
#define AWDefineStich2(DEFINE1, DEFINE2)			AWDefineStich2B(DEFINE1,DEFINE2)
#define AWDefineStich3(DEFINE1, DEFINE2, DEFINE3)	AWDefineStich3B(DEFINE1,DEFINE2,DEFINE3)
//! @endcond DOXYGEN_IGNORE


//=================================================================================================
// Various support macros
//=================================================================================================
#include "zenConstMacro.inl"

#define AWArrayCount(_Array_)						(sizeof(_Array_)/sizeof((_Array_)[0]))
#define AWMemberOffset(_Class_, _MemberName_)		( (awUInt) &(((_Class_*)0)->_MemberName_) )
#define AWMemberOffsetAfter(_Class_, _MemberName_)	( (awUInt) &(((_Class_*)0)->_MemberName_) + sizeof((((_Class_*)0)->_MemberName_)) )

#define AWClassDeclareNoParent(_Class_)				public: void AWClassDeclareCheck(){ AWStaticAssertMsg( sizeof(*this) == sizeof(_Class_), "Wrong Class name in ClassDeclare macro" );}
#define AWClassDeclare(_Class_, _ClassParent_)		AWClassDeclareNoParent(_Class_)			\
													private: typedef _ClassParent_ Super;

//=================================================================================================
// Useful compile-time infos on type/class
//=================================================================================================
//! @brief Template magic determining if parameter is a pointer (at compile time)
template<typename T> struct AWIsPointer					{ enum {value = false}; };	
template<typename T> struct AWIsPointer<T*>				{ enum {value = true};	};	//!< @brief Template specialization detecting pointers

//! @brief	Template magic to determining if parameter type/class supports memcopy
//! @detail We can safely do memcopy on builtin type and pointers without problem.
//!			Classes that contains more than builtin type (int, float, ...) are not able to
//!			use memcopy for assignment, since their operator= might be doing some more complex work.
//!			If a class is safe, it should use the macro AWAddSupportsMemCopy to make the compiler know.
template<typename T> struct AWSupportsMemCopy			{ enum {value = false}; };	
template<typename T> struct AWSupportsMemCopy<T*>		{ enum {value = true};	};
template<typename T> struct AWSupportsMemCopy<const T*>	{ enum {value = true};	};
#define AWAddSupportsMemCopy(_CLASS_)	template<> struct AWSupportsMemCopy<_CLASS_>	{ enum {value = true};	};


#include AWHeaderPlatform(zenConstMacro)

#endif
