#pragma once
#ifndef __zCore_Gfx_All_h__
#define __zCore_Gfx_All_h__

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
namespace zcGfx
{
enum eGPUPipelineMode
{
	keGpuPipe_DataUpdate,											keGpuPipe_DataUpdateAfter,
	keGpuPipe_PreDrawCompute	= keGpuPipe_DataUpdateAfter,		keGpuPipe_PreDrawComputeAfter,
	keGpuPipe_Vertex			= keGpuPipe_PreDrawComputeAfter,	keGpuPipe_VertexAfter,		 
	keGpuPipe_VertexPixel		= keGpuPipe_VertexAfter,			keGpuPipe_VertexPixelAfter, 
	keGpuPipe_PostDrawCompute	= keGpuPipe_VertexPixelAfter,		keGpuPipe_PostDrawComputeAfter,
	keGpuPipe__Count			= keGpuPipe_PostDrawComputeAfter,
	keGpuPipe__Invalid			= keGpuPipe__Count
};
}

#include "zcGfxScopedDrawlist.h"
#include "zcGfxDrawcall.h"
#include "zcGfxGPUContext.h"
#include "zcGfxRenderMgr.h"

#endif
