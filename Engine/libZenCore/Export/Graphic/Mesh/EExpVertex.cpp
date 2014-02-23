#include "libZenCore.h"

namespace EExp
{

//=================================================================================================
//! @brief		Create a new Vertex Buffer
//! @details	Create a SerialItem to be serialized and used for creating a new Resource afterward
//-------------------------------------------------------------------------------------------------
//! @param _Streams			- Infos on the various streams for the vertex
//! @param _uCreationFlags	- Type of buffer (SerialVertex_Base::eCreationFlags)
//! @return 				- Unique awResourceID of created Resource
//=================================================================================================
awResourceID CreateGfxVertex(const awArrayBase<awres::awGfxVertex::Stream>& _aStreams, awU32 _uCreationFlags)
{	
	static CMem::PoolAllocator sMemPool("Pool CreateVertexBuffer", sizeof(SerialVertex_Base::ExportInfo), 1, 5 );
	SerialVertex_Base::ExportInfo* pExportInfo	= AWNew(&sMemPool) SerialVertex_Base::ExportInfo;
	pExportInfo->maStreams						= _aStreams;
	pExportInfo->muCreationFlags				= _uCreationFlags;
	return EMgr::Export.CreateItem( awResourceID::kePlatformType_GFX, awconst::keResType_GfxVertex, pExportInfo );
}

}
