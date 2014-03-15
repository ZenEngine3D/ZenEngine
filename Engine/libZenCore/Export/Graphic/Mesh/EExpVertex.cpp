#include "libZenCore.h"

namespace EExp
{

//=================================================================================================
//! @brief		Create a new Vertex Buffer
//! @details	Create a SerialItem to be serialized and used for creating a new Resource afterward
//-------------------------------------------------------------------------------------------------
//! @param _Streams			- Infos on the various streams for the vertex
//! @param _ResourceUse		- Planned useage
//! @return 				- Unique zResID of created Resource
//=================================================================================================
zResID CreateGfxVertex(const zArrayBase<zenRes::zGfxVertex::Stream>& _aStreams, zFlagResUse _ResourceUse)
{	
	static zenMem::zAllocatorPool sMemPool("Pool CreateVertexBuffer", sizeof(SerialVertex_Base::ExportInfo), 1, 5 );
	SerialVertex_Base::ExportInfo* pExportInfo	= zenNew(&sMemPool) SerialVertex_Base::ExportInfo;
	pExportInfo->maStreams						= _aStreams;
	pExportInfo->mResourceUse					= _ResourceUse;
	return EMgr::Export.CreateItem( zResID::kePlatformType_GFX, zenConst::keResType_GfxVertex, pExportInfo );
}

}
