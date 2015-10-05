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
	ZENAssert( _ParentParamDefID.GetType() == zenConst::keResType_GfxShaderParamDef );	
	static zenMem::zAllocatorPool sMemPool("Pool CreateShaderParam", sizeof(ExportInfoGfxShaderParam), 1, 5 );
	ExportInfoGfxShaderParam*	pExportInfo		= zenNew(&sMemPool) ExportInfoGfxShaderParam;
	pExportInfo->mParentParamDefID				= _ParentParamDefID;
	return zcMgr::Export.CreateItem( zResID::kePlatformType_GFX, zenConst::keResType_GfxShaderParam, pExportInfo );
}

//=================================================================================================
//! @brief		Create a new Constant Buffer
//! @details	Create a temporary ResourceData, used for creating a new Resource and then deleted.
//!				The temporary ResourceData and Asset created, will be for engine default renderer type
//-------------------------------------------------------------------------------------------------
//! @param _ParentShaderID		- Shader resource which contains the ShaderParamDef
//! @param _eShaderParamIndex	- Which one of the ShaderParamDef to use to create an instance
//! @param _aParamValues		- Values assigned to instance (overriding default values)
//! @return 					- Unique zResID of created Resource
//=================================================================================================
zResID CreateGfxShaderParam(zResID _ParentShaderID, zcExp::eShaderParamFreq _eShaderParamIndex)
{
	ZENAssert( zenConst::kFlagResShaders.Any(_ParentShaderID.GetType()) );
	zEngineConstRef<zcRes::GfxShaderResData> rShaderData = zcDepot::ResourceData.GetItem<zcRes::GfxShaderResData>(_ParentShaderID); 
	if( rShaderData.IsValid() && rShaderData->maParamDefID[_eShaderParamIndex].IsValid() )
		return CreateGfxShaderParam( rShaderData->maParamDefID[_eShaderParamIndex]);	
	return zResID();
}

}

