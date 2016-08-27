#include "zcCore.h"

namespace zcExp
{

//=================================================================================================
//! @details	Create a new structured buffer shader resource, 
//!				with some data to initialize the gpu memory with	
//-------------------------------------------------------------------------------------------------
//! @param _pData		- Format of raw data
//! @param _uDataSize	- Total amount of data used for init
//! @param _uElemSize	- Mask of TextureFlags used for texture creation
//! @param _uElemCount	- Width/Height of input image
//! @param _UseFlags	- Data access type
//! @return 			- Unique zResID of created Resource
//=================================================================================================
zResID CreateGfxBuffer(const zU8* _pData, zUInt _uDataSize, zUInt _uElemSize, zU32 _uElemCount/*, zFlagResTexUse _UseFlags*/)
{
	static zenMem::zAllocatorPool sMemPool("Pool CreateTexture2D", sizeof(ExportInfoGfxBuffer), 1, 5 );
	ExportInfoGfxBuffer* pExportInfo	= zenNew(&sMemPool) ExportInfoGfxBuffer;	
	pExportInfo->maData.Copy(_pData, _uDataSize);
	pExportInfo->muElementSize			= _uElemSize;
	pExportInfo->muElementCount			= _uElemCount;
//	pExportInfo->mUseFlags				= _UseFlags;
	return zcMgr::Export.CreateItem( zResID::kePlatformType_GFX, zenConst::keResType_GfxBuffer, pExportInfo );
}

}
