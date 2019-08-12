#include "zcCore.h"

namespace zcExp
{

//=================================================================================================
//! @brief		Create a new 16bits Index Buffer
//! @details	Create a temporary ResourceData to create a new Resource and then delete the ResourceData.
//!				The temporary ResourceData and Asset created, will be for engine default renderer type
//! @todo Optim: Create reference to array, when executed synchronously (instead of duplicating info)
//-------------------------------------------------------------------------------------------------
//! @param _Indices			- List of Indices
//! @param _PrimType		- Type of primitive this list of indices represents (TriangleStrip, ...)
//! @return 				- Unique zResID of created Resource
//=================================================================================================
zResID CreateGfxIndexBuffer( const zArray<zU16>& _Indices, zenConst::ePrimitiveType _ePrimitiveType )
{
	auto* pExportInfo				= zenMem::NewPool<ExportInfoGfxIndex>();
	pExportInfo->maIndice16			= _Indices;
	pExportInfo->mePrimitiveType	= _ePrimitiveType;
	return zcMgr::Export.CreateItem( zResID::kePlatformType_GFX, zenConst::keResType_GfxIndex, pExportInfo );
}

//=================================================================================================
//! @brief		Create a new 32bits Index Buffer
//! @details	Create a temporary ResourceData to create a new Resource and then delete the ResourceData.
//!				The temporary ResourceData and Asset created, will be for engine default renderer type
//! @todo Optim: Create reference to array, when executed synchronously (instead of duplicating info)
//-------------------------------------------------------------------------------------------------
//! @param _Indices			- List of Indices
//! @param _PrimType		- Type of primitive this list of indices represents (TriangleStrip, ...)
//! @return 				- Unique zResID of created Resource
//=================================================================================================
zResID CreateGfxIndexBuffer( const zArray<zU32>& _Indices, zenConst::ePrimitiveType _ePrimitiveType )
{
	auto* pExportInfo				= zenMem::NewPool<ExportInfoGfxIndex>();
	pExportInfo->maIndice32			= _Indices;
	pExportInfo->mePrimitiveType	= _ePrimitiveType;
	return zcMgr::Export.CreateItem( zResID::kePlatformType_GFX, zenConst::keResType_GfxIndex, pExportInfo );
}



}