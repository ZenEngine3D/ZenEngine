#pragma once

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
//! @file		zbBase.h
//! @brief		Base header file to include in every other header files.
//! @details	Contains basic data type definitions, etc...
//=================================================================================================
#include <ZenBase/zenBase.h>
#include "zbBaseHAL.h"
#include "Type/zbType.h"
#include "Memory/zbMem.h"
#include "System/zbSys.h"
#include "IO/zbIo.h"