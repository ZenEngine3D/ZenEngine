#include "zcCore.h"

namespace zcExp
{
	ExporterGfxMeshStripDX11_DX11::ExporterGfxMeshStripDX11_DX11(const ResDataRef& _rResData)
	: Super(_rResData.Get())
	, mrResData(_rResData)
	{
	}

	bool ExporterGfxMeshStripDX11_DX11::ExportWork(bool _bIsTHRTask)
	{
		ZENAssert(mrResData.IsValid());	
		ExportInfoGfxMeshStrip* pExportInfo	= static_cast<ExportInfoGfxMeshStrip*>(mpExportInfo);
		mrResData->mIndexBufferID			= pExportInfo->mIndexBufferID;				
		mrResData->mShaderBindingID			= pExportInfo->mShaderBindingID;
		mrResData->muVertexFirst			= pExportInfo->muVertexFirst;
		mrResData->muIndexFirst				= pExportInfo->muIndexFirst;
		mrResData->muIndexCount				= pExportInfo->muIndexCount;
		return TRUE;
	}

	bool ExporterGfxMeshStripDX11_DX11::ExportEnd()
	{
		ExportInfoGfxMeshStrip*	pExportInfo							= static_cast<ExportInfoGfxMeshStrip*>(mpExportInfo);
		zEngineConstRef<ResDataGfxShaderBinding>	rShaderBinding	= zcDepot::ResourceData.GetItem<ResDataGfxShaderBinding>(pExportInfo->mShaderBindingID);
		zEngineConstRef<ResDataGfxShaderDX11>		rVertexShader	= rShaderBinding.IsValid() ? zcDepot::ResourceData.GetItem<ResDataGfxShaderDX11>(rShaderBinding->maShaderID[zenConst::keShaderStage_Vertex]) : nullptr;
		zEngineConstRef<ExportGfxIndexDX11>			rIndexBuffer	= zcDepot::ResourceData.GetItem<ExportGfxIndexDX11>(pExportInfo->mIndexBufferID);

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
			for(zUInt paramDefIdx(0); paramDefIdx<paramDefCount; ++paramDefIdx)
			{					
				if( mrResData->maShaderParamID[paramDefIdx].GetType() == zenConst::keResType_GfxShaderParamDef )
					mrResData->maShaderParamID[paramDefIdx] = zcExp::CreateGfxShaderParam( mrResData->maShaderParamID[paramDefIdx]/*, aEmptyParamValues*/);					
			}

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
			zUInt bindCount = rShaderBinding->maTextureName.Count();
			for(zUInt textureIdx(0), textureCount(pExportInfo->maTexture.Count()); textureIdx<textureCount; ++textureIdx)
			{
				for(zUInt bindIdx(0); bindIdx < bindCount; ++bindIdx)
				{
					if( pExportInfo->maTexture[textureIdx].mhTextureName == rShaderBinding->maTextureName[bindIdx] )
					{
						const ResDataGfxShaderBinding::TextureSlot& slotInfo = rShaderBinding->maTextureBind[bindIdx];
						for(zUInt stageIdx(0); stageIdx<zenConst::keShaderStage__Count; ++stageIdx )
						{
							if( slotInfo.muCount[stageIdx] > 0 )
							{
								mrResData->maTextureID[stageIdx][slotInfo.muSlot[stageIdx]] = pExportInfo->maTexture[textureIdx].mTextureID;
								mrResData->maSamplerID[stageIdx][slotInfo.muSlot[stageIdx]] = pExportInfo->maTexture[textureIdx].mSamplerID;
							}
						}
					}
				}
			}
		
			return true;
		}

		return false;
	}

}
