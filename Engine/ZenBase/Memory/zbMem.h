#pragma once

//=================================================================================================
//! @file		libZenBaseMemory.h
//! @brief		All header files includes for @b LibCore subsection @b Memory.
//! @n@n		See @ref LibCore-Mem module for more infos.
//-------------------------------------------------------------------------------------------------
//!	@addtogroup LibCore-Mem 
//!				Includes memory management modules
//! @section 	LibCore-Mem-Brief More details
//!				This is a component of the @link LibCore Core @endlink library.
//=================================================================================================
namespace zbMem
{ 
	enum Constants 
	{ 
		kAlign=16,			//!< Minimum alignment all allocation will have
		kThreadCount=128,	//!< Highest thread index memory system support
	 }; 
} 

#include "zbMemPolicy.h"
#include "zbMemPolicyContiguous.h"
#include "zbMemPolicyPool.h"

#include "zbMemDebugTracking.h"
#include "zbMemAllocator.h"

#include "zbMemPolicyPool.inl"
#include "zbMemPolicyContiguous.inl"
