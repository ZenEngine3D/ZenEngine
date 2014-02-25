#include "libZenCore.h"

namespace EExp
{

//=================================================================================================
//! @brief		Create a new Shader Parameter Definition
//! @details	
//-------------------------------------------------------------------------------------------------
//! @param _ParentShaderID	- ResousrceID of shader to extract Constant buffer Infos
//! @param _eBufferIndex	- Which Constant Buffer to export
//! @return 				- Unique zenResID of created Resource
//=================================================================================================
zenResID CreateGfxShaderParamDef( zenResID _ParentShaderID, EExp::eShaderParamFreq _eBufferIndex )
{
	static zbMem::PoolAllocator sMemPool("Pool CreateShaderParamDef", sizeof(SerialShaderParamDef_Base::ExportInfo), 1, 5 );
	SerialShaderParamDef_Base::ExportInfo* pExportInfo	= zenNew(&sMemPool) SerialShaderParamDef_Base::ExportInfo;	
	pExportInfo->mParentShaderID						= _ParentShaderID;
	pExportInfo->meBufferIndex							= _eBufferIndex;		
	return EMgr::Export.CreateItem( zenResID::kePlatformType_GFX, awconst::keResType_GfxShaderParamDef, pExportInfo );
}

}

