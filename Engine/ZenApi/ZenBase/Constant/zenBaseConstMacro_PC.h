#pragma once

//=================================================================================================
//! @file		CTypDefines_PC.h
//! @brief		Additionals defines constants for PC platform
//! @details	
//=================================================================================================

#define ZENAlign(_VAR_,_BYTESALIGN_)	__declspec(align(_BYTESALIGN_)) _VAR_
#define zenInlineForce					__forceinline 
#ifdef _MSC_VER 
#define zenRestrict						__restrict
#endif
