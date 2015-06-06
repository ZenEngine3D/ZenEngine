#include "zcCore.h"

namespace zcExp
{
	ExporterGfxShaderBindingDX11_DX11::ExporterGfxShaderBindingDX11_DX11(const ExportDataRef& _rExportData)
	: Super(_rExportData.GetSafe())
	, mrExportData(_rExportData)
	{
	}

	//=================================================================================================
	//! @brief		Assign Texture and Sampler values
	//! @details	
	//-------------------------------------------------------------------------------------------------
	//! @return		True if successful
	//=================================================================================================
	bool ExporterGfxShaderBindingDX11_DX11::ExportStart()
	{
		if( !Super::ExportStart() )
			return false;

		ExportInfoGfxShaderBinding* pExportInfo = static_cast<ExportInfoGfxShaderBinding*>(mpExportInfo);	

		mrExportData->maShaderID = pExportInfo->maShaderID;
		mdStagePerParamDef.Export( *(zArrayStatic<zHash64>*)&mrExportData->maParamDefID, mrExportData->maStagePerParamDef);

		//---------------------------------------------------------------------
		// Process texture binding infos for each shader stage
		//---------------------------------------------------------------------
		zMap<ExportDataGfxShaderBinding::TextureSlot>::Key32 dTextureBindInfo(16);
		dTextureBindInfo.SetDefaultValue( ExportDataGfxShaderBinding::TextureSlot() );
		for( zUInt stageIdx(0), stageCount(mrExportData->maShaderID.Count()); stageIdx<stageCount; ++stageIdx)
		{
			zcExp::ExportDataGfxShaderDX11* pSerialShader = EMgr::SerialItems.GetItem<zcExp::ExportDataGfxShaderDX11>( mrExportData->maShaderID[stageIdx] );
			if( pSerialShader )
			{				
				for(zUInt idxTexShader=0; idxTexShader < pSerialShader->maTextureSamplerSlot.Count(); ++idxTexShader)
				{						
					ExportDataGfxShaderDX11::BindInfo& SlotInfoIn			= pSerialShader->maTextureSamplerSlot[idxTexShader];
 				 	zHash32 hTextureName									= pSerialShader->maTextureSamplerName[idxTexShader];					
 					ExportDataGfxShaderBinding::TextureSlot& SlotInfoOut	= dTextureBindInfo.GetAdd(hTextureName);
 					SlotInfoOut.muSlot[stageIdx]							= SlotInfoIn.uSlot;
 					SlotInfoOut.muCount[stageIdx]							= SlotInfoIn.uCount;	
				}
			}
		}	
		dTextureBindInfo.Export( mrExportData->maTextureName, mrExportData->maTextureBind );

		//---------------------------------------------------------------------
		// Process parameters binding infos (list param name in each bound ParamDef)
		//---------------------------------------------------------------------	
		zMap<zU32>::Key32	dParamDefPerName(64);	
		dParamDefPerName.SetDefaultValue(0);
		for(zUInt paramDefIdx(0), paramDefCount(mrExportData->maParamDefID.Count()); paramDefIdx<paramDefCount; ++paramDefIdx )
		{
			ZENAssert( paramDefIdx < mrExportData->maParameterMask.SizeElement() );			
			const ExportDataGfxShaderParamDefDX11* pParamDef = EMgr::SerialItems.GetItem<const ExportDataGfxShaderParamDefDX11>( mrExportData->maParamDefID[paramDefIdx] );
			if( pParamDef )
			{
				zcExp::ShaderParamItemInfoMap::Iterator it(pParamDef->mdParameters);
				while( it.IsValid() )
				{
					dParamDefPerName.GetAdd(it.GetKey()) |= 1<<paramDefIdx;
					++it;
				}
			}
			else
			{
				//! @todo Missing: export error, missing ParamDef
			}
		}
		dParamDefPerName.Export( mrExportData->maParameterName, mrExportData->maParameterMask );
		ExportSkipWork();
		return true;
	}


}
