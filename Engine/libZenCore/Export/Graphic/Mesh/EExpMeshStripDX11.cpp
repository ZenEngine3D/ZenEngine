#include "libZenCore.h"
#if AW_EXPORT_OR_RESOURCE_DX11

namespace EExp
{
	bool SerialGfxMeshStrip_DX11::ExportWork(bool _bIsTHRTask)
	{
		ExportInfo* pExportInfo		= static_cast<ExportInfo*>(mpExportInfo);
		mSerial.mIndexBufferID		= pExportInfo->mIndexBufferID;				
		mSerial.mShaderBindingID	= pExportInfo->mShaderBindingID;
		muIndexFirst				= pExportInfo->muIndexFirst;
		muIndexCount				= pExportInfo->muIndexCount;
		return TRUE;
	}

	bool SerialGfxMeshStrip_DX11::ExportEnd()
	{
		ExportInfo* pExportInfo								= static_cast<ExportInfo*>(mpExportInfo);		
		EExp::SerialGfxIndex_DX11* pIndexItem				= EMgr::SerialItems.GetItem<EExp::SerialGfxIndex_DX11>(mSerial.mIndexBufferID);
		EExp::SerialGfxShaderBinding_DX11* pShaderBinding	= EMgr::SerialItems.GetItem<EExp::SerialGfxShaderBinding_DX11>(mSerial.mShaderBindingID);
		EExp::SerialGfxShaderVertex_DX11* pVertexShader		= pShaderBinding ? EMgr::SerialItems.GetItem<EExp::SerialGfxShaderVertex_DX11>(pShaderBinding->mSerial.maShaderID[keShaderStage_Vertex]) : NULL;
		if( mpExportInfo->IsSuccess() && Super::ExportEnd() && pIndexItem && pShaderBinding && pVertexShader )
		{			
			//-----------------------------------------------------------------
			// Create all needed InputStream binding
			//-----------------------------------------------------------------
			muIndexCount				= zenMath::Min( muIndexCount, pIndexItem->muIndiceCount-muIndexFirst );
			mSerial.mStreamBindingID	= CreateGfxInputStream(pExportInfo->mVertexBufferID, pVertexShader->mSerial.mShaderInputSignatureID);

			//---------------------------------------------------------------------
			// Make sure there's a ShaderParam for each ShaderParamDef needed
			//	Store ParamDefID in array, and replace it with the ParamId 
			//	associated as we find them. Then create the one missing.
			//---------------------------------------------------------------------
			// Assign all ParameterInstance provided
			mSerial.maShaderParamID	= pShaderBinding->mSerial.maParamDefID;
			awUInt paramDefCount		= mSerial.maShaderParamID.Count();
			for(awUInt paramInstIdx(0), paramInstCount(pExportInfo->maShaderParamID.Count()); paramInstIdx<paramInstCount; ++paramInstIdx )
			{
				EExp::SerialGfxShaderParam_DX11* pParamInst = EMgr::SerialItems.GetItem<EExp::SerialGfxShaderParam_DX11>(pExportInfo->maShaderParamID[paramInstIdx]);
				if( pParamInst )
				{
					for(awUInt paramDefIdx(0); paramDefIdx<paramDefCount; ++paramDefIdx)
					{					
						if( mSerial.maShaderParamID[paramDefIdx] == pParamInst->mSerial.mParentParamDefID )
							mSerial.maShaderParamID[paramDefIdx] = pParamInst->mResID;
					}
				}
			}
			// Create new empty ParameterInstance for the one not provided for needed ParameterDef
			awArrayStatic<const EExp::ParameterBase*> aEmptyParamValues;
			for(awUInt paramDefIdx(0); paramDefIdx<paramDefCount; ++paramDefIdx)
			{					
				if( mSerial.maShaderParamID[paramDefIdx].Type() == awconst::keResType_GfxShaderParamDef )
					mSerial.maShaderParamID[paramDefIdx] = EExp::CreateGfxShaderParam( mSerial.maShaderParamID[paramDefIdx], aEmptyParamValues);					
			}
			
			//---------------------------------------------------------------------
			// Make sure we have all texture assigned
			//---------------------------------------------------------------------
			// Assign default invalid value to all needed texture slot
			mSerial.maTextureID.SetCount( EExp::keShaderStage__Count );
			mSerial.maSamplerID.SetCount( EExp::keShaderStage__Count );
			for( awUInt stageIdx(0); stageIdx<EExp::keShaderStage__Count; ++stageIdx)
			{
				awUInt textureCount = pShaderBinding->maTextureBind.Count();
				mSerial.maTextureID[stageIdx].SetCount( textureCount );
				mSerial.maSamplerID[stageIdx].SetCount( textureCount );
				for( awUInt textureIdx(0); textureIdx<textureCount; ++textureIdx )
				{
					mSerial.maTextureID[stageIdx][textureIdx] = awResourceID();
					mSerial.maSamplerID[stageIdx][textureIdx] = awResourceID();
				}
			}
				
			// Now assign provided texture value
			Hashmap<EExp::SerialGfxShaderBinding_DX11::TextureSlot>::Key32 dTextureBindInfo(16);
			dTextureBindInfo.Import( pShaderBinding->maTextureName, pShaderBinding->maTextureBind );
			dTextureBindInfo.SetDefaultValue(EExp::SerialGfxShaderBinding_DX11::TextureSlot());
			for( awUInt textureIdx(0), textureCount(pExportInfo->maTexture.Count()); textureIdx<textureCount; ++textureIdx)
			{
				const EExp::SerialGfxShaderBinding_DX11::TextureSlot& slotInfo = dTextureBindInfo[ pExportInfo->maTexture[textureIdx].mhTextureName ];
				for(awUInt stageIdx(0); stageIdx<EExp::keShaderStage__Count; ++stageIdx )
				{
					if( slotInfo.muCount[stageIdx] > 0 )
					{
						mSerial.maTextureID[stageIdx][slotInfo.muSlot[stageIdx]] = pExportInfo->maTexture[textureIdx].mTextureID;
						mSerial.maSamplerID[stageIdx][slotInfo.muSlot[stageIdx]] = pExportInfo->maTexture[textureIdx].mSamplerID;
					}
				}
			}

			return true;
		}

		return false;
	}

	bool SerialGfxMeshStrip_DX11::Serialize( EExp::Serializer_Base& _Serializer )
	{
		if( Super::Serialize(_Serializer) )
			return false;

		return false;
	}
}

#endif //AW_EXPORT_OR_RESOURCE_DX11
