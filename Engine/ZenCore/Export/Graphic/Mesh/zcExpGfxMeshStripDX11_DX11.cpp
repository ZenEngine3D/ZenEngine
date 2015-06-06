#include "zcCore.h"

namespace zcExp
{
	ExporterGfxMeshStripDX11_DX11::ExporterGfxMeshStripDX11_DX11(const ExportDataRef& _rExportData)
	: Super(_rExportData.GetSafe())
	, mrExportData(_rExportData)
	{
	}

	bool ExporterGfxMeshStripDX11_DX11::ExportWork(bool _bIsTHRTask)
	{
		ZENAssert(mrExportData.IsValid());	
		ExportInfoGfxMeshStrip* pExportInfo	= static_cast<ExportInfoGfxMeshStrip*>(mpExportInfo);
		mrExportData->mIndexBufferID			= pExportInfo->mIndexBufferID;				
		mrExportData->mShaderBindingID			= pExportInfo->mShaderBindingID;
		mrExportData->muIndexFirst				= pExportInfo->muIndexFirst;
		mrExportData->muIndexCount				= pExportInfo->muIndexCount;
		return TRUE;
	}

	bool ExporterGfxMeshStripDX11_DX11::ExportEnd()
	{
		ExportInfoGfxMeshStrip*	pExportInfo		= static_cast<ExportInfoGfxMeshStrip*>(mpExportInfo);
		ExportDataGfxShaderBinding*	pShaderBinding	= EMgr::SerialItems.GetItem<ExportDataGfxShaderBinding>(pExportInfo->mShaderBindingID);
		ExportDataGfxShaderDX11*	pVertexShader	= pShaderBinding ? EMgr::SerialItems.GetItem<ExportDataGfxShaderDX11>(pShaderBinding->maShaderID[zenConst::keShaderStage_Vertex]) : NULL;
		ExportDataGfxIndexDX11*		pIndexBuffer	= EMgr::SerialItems.GetItem<ExportDataGfxIndexDX11>(pExportInfo->mIndexBufferID);

		if( mpExportInfo->IsSuccess() && Super::ExportEnd() && pIndexBuffer && pShaderBinding && pVertexShader )
		{			
			//-----------------------------------------------------------------
			// Create all needed InputStream binding
			//-----------------------------------------------------------------
			mrExportData->muIndexCount		= zenMath::Min( mrExportData->muIndexCount, pIndexBuffer->muIndiceCount-mrExportData->muIndexFirst );
			mrExportData->mStreamBindingID	= CreateGfxInputStream(pExportInfo->mVertexBufferID, pVertexShader->mShaderInputSignatureID);

			//---------------------------------------------------------------------
			// Make sure there's a ShaderParam for each ShaderParamDef needed
			//	Store ParamDefID in array, and replace it with the ParamId 
			//	associated as we find them. Then create the one missing.
			//---------------------------------------------------------------------
			// Assign all ParameterInstance provided
			mrExportData->maShaderParamID	= pShaderBinding->maParamDefID;
			zUInt paramDefCount				= mrExportData->maShaderParamID.Count();
			for(zUInt paramInstIdx(0), paramInstCount(pExportInfo->maShaderParamID.Count()); paramInstIdx<paramInstCount; ++paramInstIdx )
			{
				ExportDataGfxShaderParamDX11* pParamInst = EMgr::SerialItems.GetItem<ExportDataGfxShaderParamDX11>(pExportInfo->maShaderParamID[paramInstIdx]);
				if( pParamInst )
				{
					for(zUInt paramDefIdx(0); paramDefIdx<paramDefCount; ++paramDefIdx)
					{					
						if( mrExportData->maShaderParamID[paramDefIdx] == pParamInst->mParentParamDefID )
							mrExportData->maShaderParamID[paramDefIdx] = pParamInst->mResID;
					}
				}
			}
			// Create new empty ParameterInstance for the one not provided for needed ParameterDef
			//zArrayStatic<const zcExp::ParameterBase*> aEmptyParamValues;
			for(zUInt paramDefIdx(0); paramDefIdx<paramDefCount; ++paramDefIdx)
			{					
				if( mrExportData->maShaderParamID[paramDefIdx].Type() == zenConst::keResType_GfxShaderParamDef )
					mrExportData->maShaderParamID[paramDefIdx] = zcExp::CreateGfxShaderParam( mrExportData->maShaderParamID[paramDefIdx]/*, aEmptyParamValues*/);					
			}
			
			//---------------------------------------------------------------------
			// Make sure we have all texture assigned
			//---------------------------------------------------------------------
			// Assign default invalid value to all needed texture slot
			mrExportData->maTextureID.SetCount( zenConst::keShaderStage__Count );
			mrExportData->maSamplerID.SetCount( zenConst::keShaderStage__Count );
			for( zUInt stageIdx(0); stageIdx<zenConst::keShaderStage__Count; ++stageIdx)
			{
				zUInt textureCount = pShaderBinding->maTextureBind.Count();
				mrExportData->maTextureID[stageIdx].SetCount( textureCount );
				mrExportData->maSamplerID[stageIdx].SetCount( textureCount );
				for( zUInt textureIdx(0); textureIdx<textureCount; ++textureIdx )
				{
					mrExportData->maTextureID[stageIdx][textureIdx] = zResID();
					mrExportData->maSamplerID[stageIdx][textureIdx] = zResID();
				}
			}
				
			// Now assign provided texture value
			zMap<ExportDataGfxShaderBinding::TextureSlot>::Key32 dTextureBindInfo(16);
			dTextureBindInfo.Import( pShaderBinding->maTextureName, pShaderBinding->maTextureBind );
			dTextureBindInfo.SetDefaultValue(ExportDataGfxShaderBinding::TextureSlot());
			for( zUInt textureIdx(0), textureCount(pExportInfo->maTexture.Count()); textureIdx<textureCount; ++textureIdx)
			{
				const ExportDataGfxShaderBinding::TextureSlot& slotInfo = dTextureBindInfo[ pExportInfo->maTexture[textureIdx].mhTextureName ];
				for(zUInt stageIdx(0); stageIdx<zenConst::keShaderStage__Count; ++stageIdx )
				{
					if( slotInfo.muCount[stageIdx] > 0 )
					{
						mrExportData->maTextureID[stageIdx][slotInfo.muSlot[stageIdx]] = pExportInfo->maTexture[textureIdx].mTextureID;
						mrExportData->maSamplerID[stageIdx][slotInfo.muSlot[stageIdx]] = pExportInfo->maTexture[textureIdx].mSamplerID;
					}
				}
			}

			return true;
		}

		return false;
	}

}
