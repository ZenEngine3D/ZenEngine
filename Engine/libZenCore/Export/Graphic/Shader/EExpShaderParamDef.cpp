#include "libZenCore.h"

namespace EExp
{

//=================================================================================================
//! @brief		Create a new Shader Parameter Definition
//! @details	
//-------------------------------------------------------------------------------------------------
//! @param _ParentShaderID	- ResousrceID of shader to extract Constant buffer Infos
//! @param _eBufferIndex	- Which Constant Buffer to export
//! @return 				- Unique awResourceID of created Resource
//=================================================================================================
awResourceID CreateGfxShaderParamDef( awResourceID _ParentShaderID, EExp::eShaderParamFreq _eBufferIndex )
{
	static CMem::PoolAllocator sMemPool("Pool CreateShaderParamDef", sizeof(SerialShaderParamDef_Base::ExportInfo), 1, 5 );
	SerialShaderParamDef_Base::ExportInfo* pExportInfo	= AWNew(&sMemPool) SerialShaderParamDef_Base::ExportInfo;	
	pExportInfo->mParentShaderID						= _ParentShaderID;
	pExportInfo->meBufferIndex							= _eBufferIndex;		
	return EMgr::Export.CreateItem( awResourceID::kePlatformType_GFX, awconst::keResType_GfxShaderParamDef, pExportInfo );
}

}

