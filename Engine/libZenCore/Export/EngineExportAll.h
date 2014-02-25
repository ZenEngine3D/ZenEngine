#pragma once
#ifndef __LibExport_All_h__
#define __LibExport_All_h__

//=================================================================================================
//! @file		LibExpAll.h
//! @brief		AllocHeader file to include all sub-header files of Exporter library
//! @details	Included by platforms that can build and export data for
//!				itself and other console
//-------------------------------------------------------------------------------------------------
//!	@addtogroup LibExp			
//!				Exporter components of the engine. 
//! @n@n		Made of many components...
//!				-# @ref LibExp-Gfx @copydoc LibExp-Gfx
//-------------------------------------------------------------------------------------------------
//! @mainpage	AW Engine
//! @section	mainpage-libexp Section Exp Library
//=================================================================================================

#include <Engine/libZenBase/libZenBase.h>
#include <Engine/libZenCore/Export/EngineExportBase.h>

#include <Engine/libZenCore/Export/Serializer/ESerGetSize.h>
#include <Engine/libZenCore/Export/Serializer/ESerExportMem.h>
#include <Engine/libZenCore/Export/Serializer/ESerImportMem.h>		  
#include <Engine/libZenCore/Export/Graphic/EExpGraphicAll.h>
#include <Engine/libZenCore/Export/Export/EExpManager.h>


#endif
