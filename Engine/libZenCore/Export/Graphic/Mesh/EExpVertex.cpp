#include "libZenCore.h"

namespace EExp
{

//=================================================================================================
//! @brief		Create a new Vertex Buffer
//! @details	Create a SerialItem to be serialized and used for creating a new Resource afterward
//-------------------------------------------------------------------------------------------------
//! @param _Streams			- Infos on the various streams for the vertex
//! @param _uCreationFlags	- Type of buffer (SerialVertex_Base::eCreationFlags)
//! @return 				- Unique zenResID of created Resource
//=================================================================================================
zenResID CreateGfxVertex(const zenArrayBase<awres::awGfxVertex::Stream>& _aStreams, zenU32 _uCreationFlags)
{	
	static zbMem::PoolAllocator sMemPool("Pool CreateVertexBuffer", sizeof(SerialVertex_Base::ExportInfo), 1, 5 );
	SerialVertex_Base::ExportInfo* pExportInfo	= zenNew(&sMemPool) SerialVertex_Base::ExportInfo;
	pExportInfo->maStreams						= _aStreams;
	pExportInfo->muCreationFlags				= _uCreationFlags;
	return EMgr::Export.CreateItem( zenResID::kePlatformType_GFX, awconst::keResType_GfxVertex, pExportInfo );
}

}
