#include "libZenCore.h"

namespace EExp
{

//=================================================================================================
//! @brief		Create a new 16bits Index Buffer
//! @details	Create a temporary SerialItem to create a new Resource and then delete the SerialItem.
//!				The temporary SerialItem and Asset created, will be for engine default renderer type
//! @todo		Create reference to array, when executed synchronously (instead of duplicating info)
//-------------------------------------------------------------------------------------------------
//! @param _Indices			- List of Indices
//! @param _PrimType		- Type of primitive this list of indices represents (TriangleStrip, ...)
//! @return 				- Unique awResourceID of created Resource
//=================================================================================================
awResourceID CreateGfxIndexBuffer( const awArrayBase<awU16>& _Indices, awconst::ePrimitiveType _ePrimitiveType )
{
	static CMem::PoolAllocator sMemPool("Pool CreateIndexBuffer", sizeof(SerialIndex_Base::ExportInfo), 1, 5 );
	SerialIndex_Base::ExportInfo* pExportInfo	= AWNew(&sMemPool) SerialIndex_Base::ExportInfo;
	pExportInfo->maIndice16						= _Indices;
	pExportInfo->mePrimitiveType				= _ePrimitiveType;
	return EMgr::Export.CreateItem( awResourceID::kePlatformType_GFX, awconst::keResType_GfxIndex, pExportInfo );
}

//=================================================================================================
//! @brief		Create a new 32bits Index Buffer
//! @details	Create a temporary SerialItem to create a new Resource and then delete the SerialItem.
//!				The temporary SerialItem and Asset created, will be for engine default renderer type
//! @todo		Create reference to array, when executed synchronously (instead of duplicating info)
//-------------------------------------------------------------------------------------------------
//! @param _Indices			- List of Indices
//! @param _PrimType		- Type of primitive this list of indices represents (TriangleStrip, ...)
//! @return 				- Unique awResourceID of created Resource
//=================================================================================================
awResourceID CreateGfxIndexBuffer( const awArrayBase<awU32>& _Indices, awconst::ePrimitiveType _ePrimitiveType )
{
	static CMem::PoolAllocator sMemPool("Pool CreateIndexBuffer", sizeof(SerialIndex_Base::ExportInfo), 1, 5 );
	SerialIndex_Base::ExportInfo* pExportInfo	= AWNew(&sMemPool) SerialIndex_Base::ExportInfo;
	pExportInfo->maIndice32						= _Indices;
	pExportInfo->mePrimitiveType				= _ePrimitiveType;
	return EMgr::Export.CreateItem( awResourceID::kePlatformType_GFX, awconst::keResType_GfxIndex, pExportInfo );
}



}