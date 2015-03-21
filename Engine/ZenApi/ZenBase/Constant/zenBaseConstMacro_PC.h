#pragma once
#ifndef __zenApi_Base_Type_Defines_PC_h__
#define __zenApi_Base_Type_Defines_PC_h__

//=================================================================================================
//! @file		CTypDefines_PC.h
//! @brief		Additionals defines constants for PC platform
//! @details	
//=================================================================================================

#define ZENAlign(_VAR_,_BYTESALIGN_)	__declspec(align(_BYTESALIGN_)) _VAR_
#define ZENInlineForce					__forceinline 
#ifdef _MSC_VER 
#define ZENRestrict						__restrict
#endif

#endif
