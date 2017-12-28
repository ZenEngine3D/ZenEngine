#pragma once

//=================================================================================================
//! @file		LibGraphicAll.h
//! @brief		AllocHeader file to include all sub-header files of Graphics library
//! @details	
//-------------------------------------------------------------------------------------------------
//!	@addtogroup LibGraphic			
//!				Graphics components of the engine. 
//! @n@n		Made of many components...
//!				-# @ref LibGraphic-Renderer @copydoc LibCore-Renderer
//-------------------------------------------------------------------------------------------------
//! @mainpage	Zen Engine
//! @section	mainpage-LibGraphic Section Gfx Library
//=================================================================================================

#include zenHeaderRenderer( zcGfx )

//! @todo 1 Find proper location
//! @todo 1 rename this to phase, and use enum class
namespace zcGfx
{
enum eSubmitPhase
{
	keGpuPipe_First,
	keGpuPipe_DataUpdate,
	keGpuPipe_ComputePreDraw,
	keGpuPipe_Vertex,
	keGpuPipe_VertexPixel,
	keGpuPipe_ComputePostDraw,
	keGpuPipe_DataResolve,
	keGpuPipe_Last,

	keGpuPipe__Count,
	keGpuPipe__Invalid			= keGpuPipe__Count
};
}

#include "zcGfxCommandList.h"
#include "zcGfxDrawcall.h"
#include "zcGfxGPUContext.h"
#include "zcGfxRenderMgr.h"
