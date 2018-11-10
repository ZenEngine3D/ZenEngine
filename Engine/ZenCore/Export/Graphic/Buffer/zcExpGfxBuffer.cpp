#include "zcCore.h"

namespace zcExp
{

//=================================================================================================
//! @details	Create a new structured buffer shader resource, 
//!				with some data to initialize the gpu memory with	
//-------------------------------------------------------------------------------------------------
//! @param _pData		- Format of raw data
//! @param _uDataSize	- Total amount of data used for init
//! @param _uElemSize	- Memory size per element (stride)
//! @param _uElemCount	- Number of element to allocate memory for
//! @param _UseFlags	- Data access type
//! @return 			- Unique zResID of created Resource
//=================================================================================================
zResID CreateGfxBuffer(const zU8* _pData, size_t _uDataSize, size_t _uElemSize, zU32 _uElemCount/*, zFlagResTexUse _UseFlags*/)
{
	ExportInfoGfxBuffer* pExportInfo	= zenNewPool ExportInfoGfxBuffer;
	zenAssert(_uDataSize <= 0xFFFFFFFF);
	zenAssert(_uElemSize*_uElemCount <= 0xFFFFFFFF);
	pExportInfo->maData.Copy(_pData, _uDataSize);
	pExportInfo->muElementSize			= static_cast<zU32>(_uElemSize);
	pExportInfo->muElementCount			= _uElemCount;
//	pExportInfo->mUseFlags				= _UseFlags;
	return zcMgr::Export.CreateItem( zResID::kePlatformType_GFX, zenConst::keResType_GfxBuffer, pExportInfo );
}

}
