#include "zcCore.h"

namespace zcExp
{
	ExporterGfxMeshStripDX11_DX11::ExporterGfxMeshStripDX11_DX11(const ResDataRef& _rResData)
	: Super(_rResData.GetSafe())
	, mrResData(_rResData)
	{
	}

	bool ExporterGfxMeshStripDX11_DX11::ExportWork(bool _bIsTHRTask)
	{
		ZENAssert(mrResData.IsValid());	
		ExportInfoGfxMeshStrip* pExportInfo	= static_cast<ExportInfoGfxMeshStrip*>(mpExportInfo);
		mrResData->mIndexBufferID			= pExportInfo->mIndexBufferID;				
		mrResData->mShaderBindingID			= pExportInfo->mShaderBindingID;
		mrResData->muIndexFirst				= pExportInfo->muIndexFirst;
		mrResData->muIndexCount				= pExportInfo->muIndexCount;
		return TRUE;
	}

	bool ExporterGfxMeshStripDX11_DX11::ExportEnd()
	{
		ExportInfoGfxMeshStrip*	pExportInfo							= static_cast<ExportInfoGfxMeshStrip*>(mpExportInfo);
		zEngineConstRef<ResDataGfxShaderBinding>	rShaderBinding	= zcDepot::ResourceData.GetItem<ResDataGfxShaderBinding>(pExportInfo->mShaderBindingID);
		zEngineConstRef<ResDataGfxShaderDX11>		rVertexShader	= rShaderBinding.IsValid() ? zcDepot::ResourceData.GetItem<ResDataGfxShaderDX11>(rShaderBinding->maShaderID[zenConst::keShaderStage_Vertex]) : NULL;
		zEngineConstRef<ResDataGfxIndexDX11>		rIndexBuffer	= zcDepot::ResourceData.GetItem<ResDataGfxIndexDX11>(pExportInfo->mIndexBufferID);

		if( mpExportInfo->IsSuccess() && Super::ExportEnd() && rIndexBuffer.IsValid() && rShaderBinding.IsValid() && rVertexShader.IsValid() )
		{			
			//-----------------------------------------------------------------
			// Create all needed InputStream binding
			//-----------------------------------------------------------------
			mrResData->muIndexCount		= zenMath::Min( mrResData->muIndexCount, rIndexBuffer->muIndiceCount-mrResData->muIndexFirst );
			mrResData->mStreamBindingID	= CreateGfxInputStream(pExportInfo->mVertexBufferID, rVertexShader->mShaderInputSignatureID);

			//---------------------------------------------------------------------
			// Make sure there's a ShaderParam for each ShaderParamDef needed
			//	Store ParamDefID in array, and replace it with the ParamId 
			//	associated as we find them. Then create the one missing.
			//---------------------------------------------------------------------
			// Assign all ParameterInstance provided
			mrResData->maShaderParamID	= rShaderBinding->maParamDefID;
			zUInt paramDefCount			= mrResData->maShaderParamID.Count();
			for(zUInt paramInstIdx(0), paramInstCount(pExportInfo->maShaderParamID.Count()); paramInstIdx<paramInstCount; ++paramInstIdx )
			{
				zEngineConstRef<ResDataGfxShaderParamDX11> rParamInst = zcDepot::ResourceData.GetItem<ResDataGfxShaderParamDX11>(pExportInfo->maShaderParamID[paramInstIdx]);
				if( rParamInst.IsValid() )
				{
					for(zUInt paramDefIdx(0); paramDefIdx<paramDefCount; ++paramDefIdx)
					{					
						if( mrResData->maShaderParamID[paramDefIdx] == rParamInst->mParentParamDefID )
							mrResData->maShaderParamID[paramDefIdx] = rParamInst->mResID;
					}
				}
			}
			// Create new empty ParameterInstance for the one not provided for needed ParameterDef
			//zArrayStatic<const zcExp::ParameterBase*> aEmptyParamValues;
			for(zUInt paramDefIdx(0); paramDefIdx<paramDefCount; ++paramDefIdx)
			{					
				if( mrResData->maShaderParamID[paramDefIdx].GetType() == zenConst::keResType_GfxShaderParamDef )
					mrResData->maShaderParamID[paramDefIdx] = zcExp::CreateGfxShaderParam( mrResData->maShaderParamID[paramDefIdx]/*, aEmptyParamValues*/);					
			}
			
			//---------------------------------------------------------------------
			// Make sure we have all texture assigned
			//---------------------------------------------------------------------
			// Assign default invalid value to all needed texture slot
			mrResData->maTextureID.SetCount( zenConst::keShaderStage__Count );
			mrResData->maSamplerID.SetCount( zenConst::keShaderStage__Count );
			for( zUInt stageIdx(0); stageIdx<zenConst::keShaderStage__Count; ++stageIdx)
			{
				zUInt textureCount = rShaderBinding->maTextureBind.Count();
				mrResData->maTextureID[stageIdx].SetCount( textureCount );
				mrResData->maSamplerID[stageIdx].SetCount( textureCount );
				for( zUInt textureIdx(0); textureIdx<textureCount; ++textureIdx )
				{
					mrResData->maTextureID[stageIdx][textureIdx] = zResID();
					mrResData->maSamplerID[stageIdx][textureIdx] = zResID();
				}
			}
				
			// Now assign provided texture value
			zMap<ResDataGfxShaderBinding::TextureSlot>::Key32 dTextureBindInfo(16);
			dTextureBindInfo.Import( rShaderBinding->maTextureName, rShaderBinding->maTextureBind );
			dTextureBindInfo.SetDefaultValue(ResDataGfxShaderBinding::TextureSlot());
			for( zUInt textureIdx(0), textureCount(pExportInfo->maTexture.Count()); textureIdx<textureCount; ++textureIdx)
			{
				const ResDataGfxShaderBinding::TextureSlot& slotInfo = dTextureBindInfo[ pExportInfo->maTexture[textureIdx].mhTextureName ];
				for(zUInt stageIdx(0); stageIdx<zenConst::keShaderStage__Count; ++stageIdx )
				{
					if( slotInfo.muCount[stageIdx] > 0 )
					{
						mrResData->maTextureID[stageIdx][slotInfo.muSlot[stageIdx]] = pExportInfo->maTexture[textureIdx].mTextureID;
						mrResData->maSamplerID[stageIdx][slotInfo.muSlot[stageIdx]] = pExportInfo->maTexture[textureIdx].mSamplerID;
					}
				}
			}

			return true;
		}

		return false;
	}

}