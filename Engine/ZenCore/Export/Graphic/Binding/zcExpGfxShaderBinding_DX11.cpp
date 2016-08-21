#include "zcCore.h"

namespace zcExp
{
	ExporterGfxShaderBindingDX11_DX11::ExporterGfxShaderBindingDX11_DX11(const ExportResultRef& _rExportOut)
	: Super(_rExportOut.GetSafe())
	, mrExport(_rExportOut)
	{
		zenAssert(_rExportOut.IsValid());
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

		mrExport->maShaderID = pExportInfo->maShaderID;
		mdStagePerParamDef.Export( *(zArrayStatic<zHash64>*)&mrExport->maParamDefID, mrExport->maStagePerParamDef);

		//---------------------------------------------------------------------
		// Process texture binding infos for each shader stage
		//---------------------------------------------------------------------
		zMap<ExportGfxShaderBinding::TextureSlot>::Key32 dTextureBindInfo(16);
		dTextureBindInfo.SetDefaultValue( ExportGfxShaderBinding::TextureSlot() );
		for( zUInt stageIdx(0), stageCount(mrExport->maShaderID.Count()); stageIdx<stageCount; ++stageIdx)
		{
			zEngineConstRef<ExportGfxShaderDX11> rShaderData = zcDepot::ExportData.GetTyped<ExportGfxShaderDX11>(mrExport->maShaderID[stageIdx]);
			if( rShaderData.IsValid() )
			{				
				for(zUInt idxTexShader=0; idxTexShader < rShaderData->maTextureSamplerSlot.Count(); ++idxTexShader)
				{						
					const ExportGfxShaderDX11::BindInfo& SlotInfoIn		= rShaderData->maTextureSamplerSlot[idxTexShader];
 				 	zHash32 hTextureName								= rShaderData->maTextureSamplerName[idxTexShader];					
 					ExportGfxShaderBinding::TextureSlot& SlotInfoOut	= dTextureBindInfo.GetAdd(hTextureName);
 					SlotInfoOut.muSlot[stageIdx]						= SlotInfoIn.uSlot;
 					SlotInfoOut.muCount[stageIdx]						= SlotInfoIn.uCount;	
				}
			}
		}	
		dTextureBindInfo.Export( mrExport->maTextureName, mrExport->maTextureBind );

		//---------------------------------------------------------------------
		// Process parameters binding infos (list param name in each bound ParamDef)
		//---------------------------------------------------------------------	
		zMap<zU32>::Key32	dParamDefPerName(64);	
		dParamDefPerName.SetDefaultValue(0);
		for(zUInt paramDefIdx(0), paramDefCount(mrExport->maParamDefID.Count()); paramDefIdx<paramDefCount; ++paramDefIdx )
		{
			zenAssert( paramDefIdx < mrExport->maParameterMask.SizeItem() );			
			zEngineConstRef<ExportGfxShaderParamDefDX11> rParamDef = zcDepot::ExportData.GetTyped<ExportGfxShaderParamDefDX11>( mrExport->maParamDefID[paramDefIdx] );
			if( rParamDef.IsValid() )
			{
				zcExp::ShaderParamItemInfoMap::Iterator it(rParamDef->mdParameters);
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
		dParamDefPerName.Export( mrExport->maParameterName, mrExport->maParameterMask );
		ExportSkipWork();
		return true;
	}
}
