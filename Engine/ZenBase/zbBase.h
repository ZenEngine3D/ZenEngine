#pragma once
#ifndef __zBase_Base_h__
#define __zBase_Base_h__ 

//=================================================================================================
//! @file		libZenBaseBase.h
//! @brief		AllocHeader file to include all sub-header files of Core library
//! @details	
//-------------------------------------------------------------------------------------------------
//!	@addtogroup LibCore			
//!				Core components of the engine. 
//! @n@n		Used by other libraries with a higher level of abstraction.
//! @n@n		This library shouldn't reference anything outside of itself.				
//! @n@n		Made of many components...
//!				-# @ref LibCore-Type @copydoc LibCore-Type
//! 			-# @ref LibCore-Math @copydoc LibCore-Math
//!				-# @ref LibCore-Mem @copydoc LibCore-Mem
//!				-# @ref LibCore-Log @copydoc LibCore-Log
//!				-# @ref LibCore-Sys @copydoc LibCore-Sys
//-------------------------------------------------------------------------------------------------
//! @mainpage	Zen Engine
//! @section	mainpage-libcore Section Core Library
//=================================================================================================

//=================================================================================================
//! @file		libZenBaseBase.h
//! @brief		Base header file to include in every other header files.
//! @details	Contains basic data type definitions, etc...
//=================================================================================================
#define ZEN_ENGINELIB 1

#include <ZenBase/zenBase.h>
#include <Engine/ZenBase/Type/zbType.h>
#include <Engine/ZenBase/Memory/zbMem.h>
#include <Engine/ZenBase/System/zbSys.h>
#include <Engine/ZenBase/IO/zbIo.h>

#endif