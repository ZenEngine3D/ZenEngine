#include "zcCore.h"

namespace zcExp
{

//=================================================================================================
//! @brief		Create a new Shader Parameter instance
//! @details	
//-------------------------------------------------------------------------------------------------
//! @param _ParentParamDefID	- ResoureeID of the parent Shader ParamDefinition instantiate
//! @param _aParamValues		- Values assigned to instance (overriding default values)
//! @return 					- Unique zResID of created Resource
//=================================================================================================
zResID CreateGfxShaderParam(zResID _ParentParamDefID)
{
	ZENAssert( _ParentParamDefID.Type() == zenConst::keResType_GfxShaderParamDef );	
	static zenMem::zAllocatorPool sMemPool("Pool CreateShaderParam", sizeof(ExportInfoGfxShaderParam), 1, 5 );
	ExportInfoGfxShaderParam*	pExportInfo		= zenNew(&sMemPool) ExportInfoGfxShaderParam;
	pExportInfo->mParentParamDefID				= _ParentParamDefID;
	return EMgr::Export.CreateItem( zResID::kePlatformType_GFX, zenConst::keResType_GfxShaderParam, pExportInfo );
}

//=================================================================================================
//! @brief		Create a new Constant Buffer
//! @details	Create a temporary SerialItem, used for creating a new Resource and then deleted.
//!				The temporary SerialItem and Asset created, will be for engine default renderer type
//-------------------------------------------------------------------------------------------------
//! @param _ParentShaderID		- Shader resource which contains the ShaderParamDef
//! @param _eShaderParamIndex	- Which one of the ShaderParamDef to use to create an instance
//! @param _aParamValues		- Values assigned to instance (overriding default values)
//! @return 					- Unique zResID of created Resource
//=================================================================================================
zResID CreateGfxShaderParam(zResID _ParentShaderID, zcExp::eShaderParamFreq _eShaderParamIndex)
{
	ZENAssert( zenConst::kFlagResShaders.Any(_ParentShaderID.Type()) );
	zcRes::GfxShaderResDataRef rShaderData = EMgr::SerialItems.GetItem<zcRes::GfxShaderResData>(_ParentShaderID); 
	if( rShaderData.IsValid() && rShaderData->maParamDefID[_eShaderParamIndex].IsValid() )
	return CreateGfxShaderParam( rShaderData->maParamDefID[_eShaderParamIndex]);	
	return zResID();
}

}

