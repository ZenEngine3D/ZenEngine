#include "zcCore.h"

namespace zcExp
{

//=================================================================================================
//! @brief		Create a new Vertex Buffer
//! @details	Create a ResourceData to be serialized and used for creating a new Resource afterward
//-------------------------------------------------------------------------------------------------
//! @param _Streams			- Infos on the various streams for the vertex
//! @param _ResourceUse		- Planned useage
//! @return 				- Unique zResID of created Resource
//=================================================================================================
zResID CreateGfxVertex(const zArrayBase<zenRes::zGfxVertex::Stream>& _aStreams, zFlagResUse _ResourceUse)
{	
	static zenMem::zAllocatorPool sMemPool("Pool CreateVertexBuffer", sizeof(ExportInfoGfxVertex), 1, 5 );
	ExportInfoGfxVertex* pExportInfo	= zenNew(&sMemPool) ExportInfoGfxVertex;
	pExportInfo->maStreams				= _aStreams;
	pExportInfo->mResourceUse			= _ResourceUse;
	return EMgr::Export.CreateItem( zResID::kePlatformType_GFX, zenConst::keResType_GfxVertex, pExportInfo );
}

}
