#include "zcCore.h"

namespace zcExp
{
	ExporterGfxMeshStrip::ExporterGfxMeshStrip(const ExportResultRef& _rExportOut)
	: Super(_rExportOut.Get())
	, mrExport(_rExportOut)
	{
		zenAssert(mrExport.IsValid());
	}

	bool ExporterGfxMeshStrip::ExportStart()
	{
		if( Super::ExportStart() == false )
			return false;			

		ExportInfoGfxMeshStrip* pExportInfo						= static_cast<ExportInfoGfxMeshStrip*>(mpExportInfo);
		mrExport->mIndexBufferID								= pExportInfo->mIndexBufferID;				
		mrExport->mShaderBindingID								= pExportInfo->mShaderBindingID;
		mrExport->muVertexFirst									= pExportInfo->muVertexFirst;
		mrExport->muIndexFirst									= pExportInfo->muIndexFirst;
		mrExport->muIndexCount									= pExportInfo->muIndexCount;
		
		zEngineConstRef<ExportGfxIndex>			rIndexBuffer	= zcDepot::ExportData.GetTyped<ExportGfxIndex>(pExportInfo->mIndexBufferID);
		zEngineConstRef<ExportGfxShaderBinding>	rShaderBinding	= zcDepot::ExportData.GetTyped<ExportGfxShaderBinding>(pExportInfo->mShaderBindingID);
		zEngineConstRef<ExportGfxShader>		rVertexShader	= rShaderBinding.IsValid() ? zcDepot::ExportData.GetTyped<ExportGfxShader>(rShaderBinding->maShaderID[zenConst::keShaderStage_Vertex]) : nullptr;		

		if( rIndexBuffer.IsValid() && rShaderBinding.IsValid() && rVertexShader.IsValid() )
		{			
			//-----------------------------------------------------------------
			// Create all needed InputStream binding
			//-----------------------------------------------------------------
			mrExport->muIndexCount		= zenMath::Min( mrExport->muIndexCount, rIndexBuffer->muIndiceCount-mrExport->muIndexFirst );
			mrExport->mStreamBindingID	= CreateGfxInputStream(pExportInfo->mVertexBufferID, rVertexShader->mShaderInputSignatureID);
			 
			//---------------------------------------------------------------------
			// Make sure there's a ShaderParam for each ShaderParamDef needed
			//	Store ParamDefID in array, and replace it with the ParamId 
			//	associated as we find them. Then create the one missing.
			//---------------------------------------------------------------------
			// Assign all ParameterInstance provided
			mrExport->maShaderParamID	= rShaderBinding->maParamDefID;
			zUInt paramDefCount			= mrExport->maShaderParamID.Count();
			for(zUInt paramInstIdx(0), paramInstCount(pExportInfo->maShaderParamID.Count()); paramInstIdx<paramInstCount; ++paramInstIdx )
			{
				zEngineConstRef<ExportGfxShaderParamDX11> rParamInst = zcDepot::ExportData.GetTyped<ExportGfxShaderParamDX11>(pExportInfo->maShaderParamID[paramInstIdx]);
				if( rParamInst.IsValid() )
				{
					for(zUInt paramDefIdx(0); paramDefIdx<paramDefCount; ++paramDefIdx)
					{					
						if( mrExport->maShaderParamID[paramDefIdx] == rParamInst->mParentParamDefID )
							mrExport->maShaderParamID[paramDefIdx] = rParamInst->mResID;
					}
				}
			}
			// Create new empty ParameterInstance for the one not provided for needed ParameterDef
			for(zUInt paramDefIdx(0); paramDefIdx<paramDefCount; ++paramDefIdx)
			{					
				if( mrExport->maShaderParamID[paramDefIdx].GetType() == zenConst::keResType_GfxShaderParamDef )
					mrExport->maShaderParamID[paramDefIdx] = zcExp::CreateGfxShaderParam( mrExport->maShaderParamID[paramDefIdx]/*, aEmptyParamValues*/);					
			}

			//Find max assigned texture slot per shader stage
			mrExport->maTextureID.SetCount( zenConst::keShaderStage__Count );
			mrExport->maSamplerID.SetCount( zenConst::keShaderStage__Count );
			zUInt bindCount = rShaderBinding->maTextureName.Count();
			for( zUInt stageIdx(0); stageIdx<zenConst::keShaderStage__Count; ++stageIdx)
			{
				zUInt uMaxSlot(0);
				for(zUInt bindIdx(0); bindIdx < bindCount; ++bindIdx)
				{
					const ExportGfxShaderBinding::TextureSlot& bindInfo = rShaderBinding->maTextureBind[bindIdx];
					if( bindInfo.muCount[stageIdx] > 0 )
						uMaxSlot = zenMath::Max<zUInt>(uMaxSlot, bindInfo.muSlot[stageIdx] + bindInfo.muCount[stageIdx] );
				}				
				mrExport->maTextureID[stageIdx].SetCount( uMaxSlot );
				mrExport->maSamplerID[stageIdx].SetCount( uMaxSlot );
				for( zUInt textureIdx(0); textureIdx<uMaxSlot; ++textureIdx )
				{
					mrExport->maTextureID[stageIdx][textureIdx] = zResID();
					mrExport->maSamplerID[stageIdx][textureIdx] = zResID();
				}
			}

			// Now assign provided texture value			
			for(zUInt textureIdx(0), textureCount(pExportInfo->maTexture.Count()); textureIdx<textureCount; ++textureIdx)
			{
				for(zUInt bindIdx(0); bindIdx < bindCount; ++bindIdx)
				{
					if( pExportInfo->maTexture[textureIdx].mhTextureName == rShaderBinding->maTextureName[bindIdx] )
					{
						const ExportGfxShaderBinding::TextureSlot& slotInfo = rShaderBinding->maTextureBind[bindIdx];
						for(zUInt stageIdx(0); stageIdx<zenConst::keShaderStage__Count; ++stageIdx )
						{
							if( slotInfo.muCount[stageIdx] > 0 )
							{
								mrExport->maTextureID[stageIdx][slotInfo.muSlot[stageIdx]] = pExportInfo->maTexture[textureIdx].mTextureID;
								mrExport->maSamplerID[stageIdx][slotInfo.muSlot[stageIdx]] = pExportInfo->maTexture[textureIdx].mSamplerID;
							}
						}
					}
				}
			}

			ExportSkipWork();
			return true;
		}

		return false;
	}

	//=================================================================================================
	//! @brief		Create a new MeshStrip
	//! @details	Tie together vertex, index and shader, giving us all the info needed 
	//				to make a render call
	//-------------------------------------------------------------------------------------------------
	//! @param _VertexBufferID	- VertexBuffer Resource
	//! @param _IndexBufferID	- IndexBuffer Resource
	//! @param _ShaderBindingID	- Resource used for the shader config
	//! @param _uIndexFirst		- First index used in IndexBuffer, 
	//! @param _uIndexCount		- Number of Index to use in IndexBuffer
	//! @return 				- Created MeshStrip
	//=================================================================================================
	zResID CreateGfxMeshStrip(zResID _VertexBufferID, zResID _IndexBufferID, zResID _ShaderBindingID, zU32 _uIndexFirst, zU32 _uIndexCount, zU32 _uVertexFirst, const zArrayBase<zResID>& _aShaderParamID, const zArrayBase<zenRes::zShaderTexture>& _aTexture)
	{
		static zenMem::zAllocatorPool sMemPool("Pool CreateMeshStrip", sizeof(ExportInfoGfxMeshStrip), 1, 5 );
		ExportInfoGfxMeshStrip* pExportInfo	= zenNew(&sMemPool) ExportInfoGfxMeshStrip;
		pExportInfo->mVertexBufferID			= _VertexBufferID;	
		pExportInfo->mIndexBufferID				= _IndexBufferID;
		pExportInfo->mShaderBindingID			= _ShaderBindingID;
		pExportInfo->maTexture					= _aTexture;				
		pExportInfo->maShaderParamID			= _aShaderParamID;
		pExportInfo->muVertexFirst				= _uVertexFirst;
		pExportInfo->muIndexFirst				= _uIndexFirst;
		pExportInfo->muIndexCount				= _uIndexCount;		
		return zcMgr::Export.CreateItem( zResID::kePlatformType_GFX, zenConst::keResType_GfxMeshStrip, pExportInfo );
	}


}

