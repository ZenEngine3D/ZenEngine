#include "zcCore.h"

namespace zcExp
{

//=================================================================================================
//! @brief		Create a new Shader Parameter Definition
//! @details	
//-------------------------------------------------------------------------------------------------
//! @param _ParentShaderID	- ResousrceID of shader to extract Constant buffer Infos
//! @param _eBufferIndex	- Which Constant Buffer to export
//! @return 				- Unique zResID of created Resource
//=================================================================================================
zResID CreateGfxShaderParamDef( zResID _ParentShaderID, zcExp::eShaderParamFreq _eBufferIndex )
{
	static zenMem::zAllocatorPool sMemPool("Pool CreateShaderParamDef", sizeof(SerialShaderParamDef_Base::ExportInfo), 1, 5 );
	SerialShaderParamDef_Base::ExportInfo* pExportInfo	= zenNew(&sMemPool) SerialShaderParamDef_Base::ExportInfo;	
	pExportInfo->mParentShaderID						= _ParentShaderID;
	pExportInfo->meBufferIndex							= _eBufferIndex;		
	return EMgr::Export.CreateItem( zResID::kePlatformType_GFX, zenConst::keResType_GfxShaderParamDef, pExportInfo );
}

}

