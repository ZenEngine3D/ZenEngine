#include "libZenCore.h"
#if AW_EXPORT_OR_RESOURCE_DX11

namespace EExp
{
	bool SerialGfxShaderBinding_DX11::Serialize( EExp::Serializer_Base& _Serializer )
	{
		return FALSE; 
	}

	//=================================================================================================
	//! @brief		Assign Texture and Sampler values
	//! @details	
	//-------------------------------------------------------------------------------------------------
	//! @return		True if successful
	//=================================================================================================
	bool SerialGfxShaderBinding_DX11::ExportStart()
	{
		if( !Super::ExportStart() )
			return false;

		ExportInfo*	pExportInfo	= static_cast<ExportInfo*>(mpExportInfo);	

		mSerial.maShaderID = pExportInfo->maShaderID;
		pExportInfo->mdStagePerParamDef.Export( *(zArrayStatic<zHash64>*)&mSerial.maParamDefID, maStagePerParamDef);

		//---------------------------------------------------------------------
		// Process texture binding infos for each shader stage
		//---------------------------------------------------------------------
		zMap<TextureSlot>::Key32 dTextureBindInfo(16);
		dTextureBindInfo.SetDefaultValue( TextureSlot() );
		for( zUInt stageIdx(0), stageCount(mSerial.maShaderID.Count()); stageIdx<stageCount; ++stageIdx)
		{
			EExp::SerialShader_DX11* pSerialShader = EMgr::SerialItems.GetItem<EExp::SerialShader_DX11>( mSerial.maShaderID[stageIdx] );
			if( pSerialShader )
			{				
				for(zUInt idxTexShader=0; idxTexShader < pSerialShader->maTextureSamplerSlot.Count(); ++idxTexShader)
				{						
					SerialShader_DX11::BindInfo& SlotInfoIn	= pSerialShader->maTextureSamplerSlot[idxTexShader];
 				 	zHash32 hTextureName						= pSerialShader->maTextureSamplerName[idxTexShader];					
 					TextureSlot& SlotInfoOut				= dTextureBindInfo.GetAdd(hTextureName);
 					SlotInfoOut.muSlot[stageIdx]			= SlotInfoIn.uSlot;
 					SlotInfoOut.muCount[stageIdx]			= SlotInfoIn.uCount;	
				}
			}
		}	
		dTextureBindInfo.Export( maTextureName, maTextureBind );

		//---------------------------------------------------------------------
		// Process parameters binding infos (list param name in each bound ParamDef)
		//---------------------------------------------------------------------	
		zMap<zU32>::Key32	dParamDefPerName(64);	
		dParamDefPerName.SetDefaultValue(0);
		for(zUInt paramDefIdx(0), paramDefCount(mSerial.maParamDefID.Count()); paramDefIdx<paramDefCount; ++paramDefIdx )
		{
			ZENAssert( paramDefIdx < maParameterMask.SizeElement() );			
			const SerialGfxShaderParamDef_DX11* pParamDef = EMgr::SerialItems.GetItem<const SerialGfxShaderParamDef_DX11>( mSerial.maParamDefID[paramDefIdx] );
			if( pParamDef )
			{
				EExp::ShaderParamItemInfoMap::Iterator it(pParamDef->mdParameters);
				while( it.IsValid() )
				{
					dParamDefPerName.GetAdd(it.GetKey()) |= 1<<paramDefIdx;
					++it;
				}
			}
			else
			{
				//! @todo export error, missing ParamDef
			}
		}
		dParamDefPerName.Export( maParameterName, maParameterMask );
		ExportSkipWork();
		return true;
	}
}

#endif //AW_EXPORT_OR_RESOURCE_DX11
