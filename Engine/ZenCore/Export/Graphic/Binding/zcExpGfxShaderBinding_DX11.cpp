#include "zcCore.h"

namespace zcExp
{
	ExporterGfxShaderBindingDX11_DX11::ExporterGfxShaderBindingDX11_DX11(const ResDataRef& _rResData)
	: Super(_rResData.GetSafe())
	, mrResData(_rResData)
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

		mrResData->maShaderID = pExportInfo->maShaderID;
		mdStagePerParamDef.Export( *(zArrayStatic<zHash64>*)&mrResData->maParamDefID, mrResData->maStagePerParamDef);

		//---------------------------------------------------------------------
		// Process texture binding infos for each shader stage
		//---------------------------------------------------------------------
		zMap<ResDataGfxShaderBinding::TextureSlot>::Key32 dTextureBindInfo(16);
		dTextureBindInfo.SetDefaultValue( ResDataGfxShaderBinding::TextureSlot() );
		for( zUInt stageIdx(0), stageCount(mrResData->maShaderID.Count()); stageIdx<stageCount; ++stageIdx)
		{
			zcExp::ResDataGfxShaderDX11* pSerialShader = EMgr::SerialItems.GetItem<zcExp::ResDataGfxShaderDX11>( mrResData->maShaderID[stageIdx] );
			if( pSerialShader )
			{				
				for(zUInt idxTexShader=0; idxTexShader < pSerialShader->maTextureSamplerSlot.Count(); ++idxTexShader)
				{						
					ResDataGfxShaderDX11::BindInfo& SlotInfoIn			= pSerialShader->maTextureSamplerSlot[idxTexShader];
 				 	zHash32 hTextureName									= pSerialShader->maTextureSamplerName[idxTexShader];					
 					ResDataGfxShaderBinding::TextureSlot& SlotInfoOut	= dTextureBindInfo.GetAdd(hTextureName);
 					SlotInfoOut.muSlot[stageIdx]							= SlotInfoIn.uSlot;
 					SlotInfoOut.muCount[stageIdx]							= SlotInfoIn.uCount;	
				}
			}
		}	
		dTextureBindInfo.Export( mrResData->maTextureName, mrResData->maTextureBind );

		//---------------------------------------------------------------------
		// Process parameters binding infos (list param name in each bound ParamDef)
		//---------------------------------------------------------------------	
		zMap<zU32>::Key32	dParamDefPerName(64);	
		dParamDefPerName.SetDefaultValue(0);
		for(zUInt paramDefIdx(0), paramDefCount(mrResData->maParamDefID.Count()); paramDefIdx<paramDefCount; ++paramDefIdx )
		{
			ZENAssert( paramDefIdx < mrResData->maParameterMask.SizeElement() );			
			const ResDataGfxShaderParamDefDX11* pParamDef = EMgr::SerialItems.GetItem<const ResDataGfxShaderParamDefDX11>( mrResData->maParamDefID[paramDefIdx] );
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
		dParamDefPerName.Export( mrResData->maParameterName, mrResData->maParameterMask );
		ExportSkipWork();
		return true;
	}


}
