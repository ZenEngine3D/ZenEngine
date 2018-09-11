#include "zcCore.h"

namespace zcExp
{

//=================================================================================================
//! @details	
//-------------------------------------------------------------------------------------------------
//! @param _ParentParamDefID	- ResoureeID of the parent Shader ParamDefinition instantiate
//! @return 					- Unique zResID of created Resource
//=================================================================================================
zResID CreateGfxCBuffer(zResID _ParentParamDefID)
{
	zenAssert( _ParentParamDefID.GetType() == zenConst::keResType_GfxCBufferDefinition );	
	//static zenMem::zAllocatorPool sMemPool("Pool CreateShaderParam", sizeof(ExportInfoGfxShaderParam), 1, 5 );
	ExportInfoGfxShaderParam* pExportInfo		= zenNewPool ExportInfoGfxShaderParam;
	pExportInfo->mParentParamDefID				= _ParentParamDefID;
	return zcMgr::Export.CreateItem( zResID::kePlatformType_GFX, zenConst::keResType_GfxCBuffer, pExportInfo );
}

//=================================================================================================
//! @details	
//-------------------------------------------------------------------------------------------------
//! @param _ParentShaderID		- Shader resource which contains the ShaderParamDef
//! @param _uCBufferSlot		- Which Constant Buffer definition picked to create an instance
//! @return 					- Unique zResID of created Resource
//=================================================================================================
zResID CreateGfxCBuffer(zResID _ParentShaderID, zU8 _uCBufferSlot)
{
	zenAssert( zenConst::kFlagResShaders.Any(_ParentShaderID.GetType()) );
	zEngineConstRef<zcExp::ExportGfxShader> rShaderData = zcDepot::ExportData.GetTyped<zcExp::ExportGfxShader>(_ParentShaderID); 
	if( rShaderData.IsValid() && _uCBufferSlot < rShaderData->maCBufferParentID.Count() && rShaderData->maCBufferParentID[_uCBufferSlot].IsValid() )
		return CreateGfxCBuffer(rShaderData->maCBufferParentID[_uCBufferSlot]);	
	return zResID();
}

//=================================================================================================
//! @details	
//-------------------------------------------------------------------------------------------------
//! @param _ParentShaderID		- Shader resource which contains the ShaderParamDef
//! @param _hCBufferName		- Which Constant Buffer definition picked to create an instance
//! @return 					- Unique zResID of created Resource
//=================================================================================================
zResID CreateGfxCBuffer(zResID _ParentShaderID, zHash32 _hCBufferName)
{
	zenAssert( zenConst::kFlagResShaders.Any(_ParentShaderID.GetType()) );
	zEngineConstRef<zcExp::ExportGfxShader> rShaderData = zcDepot::ExportData.GetTyped<zcExp::ExportGfxShader>(_ParentShaderID); 	
	if( rShaderData.IsValid() )
	{
		for(zUInt resIdx(0), resCount(rShaderData->maResourceBinding.Count()); resIdx < resCount; ++resIdx)
		{
			const auto& resBinding = rShaderData->maResourceBinding[resIdx];
			if( resBinding.meType == keShaderRes_CBuffer && resBinding.mzName.mhName == _hCBufferName )
			{
				zenAssert( resBinding.muSlotIndex < rShaderData->maCBufferParentID.Count() );
				return CreateGfxCBuffer(rShaderData->maCBufferParentID[resBinding.muSlotIndex]);	
			}
		}
	}
	return zResID();
}

//=================================================================================================
//! @details Constructor
//-------------------------------------------------------------------------------------------------
//=================================================================================================
ExporterGfxCBuffer::ExporterGfxCBuffer(const ExportResultRef& _rExportOut)
: Super(_rExportOut.GetSafe())
, mrExport(_rExportOut)
{
	zenAssert(mrExport.IsValid());
}

//=================================================================================================
//! @details Create an empty CBuffer, init with default values defined in its CBufferDefinition	
//-------------------------------------------------------------------------------------------------
//! @return 					- True if creation succeeded
//=================================================================================================
bool ExporterGfxCBuffer::ExportStart()
{
	ExportSkipWork();
	if( Super::ExportStart() )
	{
		ExportInfoGfxShaderParam* pExportInfo					= static_cast<ExportInfoGfxShaderParam*>(mpExportInfo);
		mrExport->mParentParamDefID								= pExportInfo->mParentParamDefID;	
		zEngineConstRef<ExportGfxCBufferDefinition> rParent		= zcDepot::ExportData.GetTyped<ExportGfxCBufferDefinition>(pExportInfo->mParentParamDefID);
		if( rParent.IsValid() )
		{
			mrExport->maParameterValues = rParent->muDefaultValues;
			return TRUE;
		}
	}
	return FALSE;
}

}

