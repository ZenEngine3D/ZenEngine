#pragma once
#ifndef __LibCore_Type_Defines_PC_h__
#define __LibCore_Type_Defines_PC_h__

//=================================================================================================
//! @file		CTypDefines_PC.h
//! @brief		Additionals defines constants for PC platform
//! @details	
//=================================================================================================

#define AWAlign(_VAR_,_BYTESALIGN_)	__declspec(align(_BYTESALIGN_)) _VAR_
#define AWForceInline				__forceinline 
#ifdef _MSC_VER 
#define AWRestrict					__restrict
#endif


#endif