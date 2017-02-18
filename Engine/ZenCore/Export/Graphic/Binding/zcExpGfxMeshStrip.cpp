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

		ExportInfoGfxMeshStrip*							pExportInfo	= static_cast<ExportInfoGfxMeshStrip*>(mpExportInfo);
		const zArrayStatic<zenRes::zShaderResource>&	aResources	= pExportInfo->maResources;
		zEngineConstRef<ExportGfxShader>				arShader[keShaderStage__Count];

		mrExport->mIndexBufferID									= pExportInfo->mIndexBufferID;				
		mrExport->mShaderBindingID									= pExportInfo->mShaderBindingID;
		mrExport->muVertexFirst										= pExportInfo->muVertexFirst;
		mrExport->muIndexFirst										= pExportInfo->muIndexFirst;
		mrExport->muIndexCount										= pExportInfo->muIndexCount;
		
		zEngineConstRef<ExportGfxIndex>			rIndexBuffer		= zcDepot::ExportData.GetTyped<ExportGfxIndex>(pExportInfo->mIndexBufferID);
		zEngineConstRef<ExportGfxShaderBinding>	rShaderBinding		= zcDepot::ExportData.GetTyped<ExportGfxShaderBinding>(pExportInfo->mShaderBindingID);		
		if( rIndexBuffer.IsValid() && rShaderBinding.IsValid() )
		{						
			zUInt cbuffDefCount			= rShaderBinding->maCBufferParentID.Count();
			mrExport->muIndexCount		= zenMath::Min( mrExport->muIndexCount, rIndexBuffer->muIndiceCount-mrExport->muIndexFirst );
			mrExport->maConstanBufferID.SetCount( cbuffDefCount );

			//-------------------------------------------------------------------------------------
			// Init Shaders input slots
			for( zUInt stageIdx(0); stageIdx<keShaderStage__Count; ++stageIdx)
			{
				arShader[stageIdx] = zcDepot::ExportData.GetTyped<ExportGfxShader>( rShaderBinding->maShaderID[stageIdx] );
				if( arShader[stageIdx].IsValid() )
				{					
					for(zUInt resType(0); resType<keShaderRes__Count; ++resType)
						mrExport->maResourceID[stageIdx][resType].SetCount( arShader[stageIdx]->maResourceBindMax[resType] );
				}
			}

			//-------------------------------------------------------------------------------------
			// Assign provided resources to input slots			
			for(zUInt resIdx(0), resCount(pExportInfo->maResources.Count()); resIdx<resCount; ++resIdx)
			{	
				const zResID ResID = aResources[resIdx].mResourceID;
				if( ResID.IsValid() )
				{
					ExportGfxShaderBinding::ShaderBindInfoIndex resInfo;
					zHash32 zResName		= aResources[resIdx].mhResourceName;									
					bool bValidAssignedRes	= rShaderBinding->mdResourceBind.Get(zResName, resInfo);
				
					// Special case for CBuffer, also need to assign ResID in another array and find it by parent CBufferDefinition 
					if( ResID.GetType() == keResType_GfxCBuffer )
					{
						zEngineConstRef<ExportGfxCBuffer> rCBuffer = zcDepot::ExportData.GetTyped<ExportGfxCBuffer>(ResID);
						if( rCBuffer.IsValid() )
						{
							zResID parentResId = rCBuffer->mParentParamDefID;
							for(zUInt cbuffDefIdx(0); cbuffDefIdx<cbuffDefCount && !bValidAssignedRes; ++cbuffDefIdx )
							{
								if( parentResId == rShaderBinding->maCBufferParentID[cbuffDefIdx] )
								{
									zU8 uResBindIndex							= rShaderBinding->maCBufferParentBindIndex[cbuffDefIdx];
									resInfo										= rShaderBinding->maResourceBind[uResBindIndex];
									mrExport->maConstanBufferID[cbuffDefIdx]	= ResID;
									bValidAssignedRes							= true;								
								}
							}
						}
					}
					// Assign resource to resource array
					if( bValidAssignedRes )
					{						
						for( zUInt stageIdx(0); stageIdx<keShaderStage__Count; ++stageIdx)
						{
							//! @todo safe make sure the resource is proper size/type
							auto resIndex = resInfo.muShaderResIndex[stageIdx];
							if( arShader[stageIdx].IsValid() && resIndex < arShader[stageIdx]->maResourceBinding.Count() )
							{
								const ExportGfxShader::ShaderBindInfo& bindInfo							= arShader[stageIdx]->maResourceBinding[ resIndex ];
								zenAssert( bindInfo.muSlotIndex < mrExport->maResourceID[stageIdx][bindInfo.meType].Count() );
								mrExport->maResourceID[stageIdx][bindInfo.meType][bindInfo.muSlotIndex] = ResID;							
							}
						}
					}
				}
			}

			//-------------------------------------------------------------------------------------
			// Make sure there's a ConstantBuffer for each ConstantBufferDef used in shaders.
			// If not provided in resources export config, create the one missing.
			for(zUInt cbuffDefIdx(0); cbuffDefIdx<cbuffDefCount; ++cbuffDefIdx)
			{					
				if( mrExport->maConstanBufferID[cbuffDefIdx].IsValid() == false )
				{
					// Create the CBuffer
					zResID CBufferDefID											= rShaderBinding->maCBufferParentID[cbuffDefIdx];
					const auto IdxRes											= rShaderBinding->maCBufferParentBindIndex[cbuffDefIdx];
					const ExportGfxShaderBinding::ShaderBindInfoIndex& resInfo	= rShaderBinding->maResourceBind[IdxRes];
					mrExport->maConstanBufferID[cbuffDefIdx]					= zcExp::CreateGfxCBuffer(CBufferDefID);
					
					// Assign it to shaders resources
					for( zUInt stageIdx(0); stageIdx<keShaderStage__Count; ++stageIdx)
					{
						zEngineRef<ExportGfxShader> rShader = zcDepot::ExportData.GetTyped<ExportGfxShader>( rShaderBinding->maShaderID[stageIdx] );
						auto resIndex						= resInfo.muShaderResIndex[stageIdx];
						if( arShader[stageIdx].IsValid() && resIndex < arShader[stageIdx]->maResourceBinding.Count() )
						{
							zcExp::ExportGfxShader::ShaderBindInfo& BindInfo = rShader->maResourceBinding[ resIndex ];
							mrExport->maResourceID[stageIdx][keShaderRes_CBuffer][BindInfo.muSlotIndex] = mrExport->maConstanBufferID[cbuffDefIdx];
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
	zResID CreateGfxMeshStrip(zResID _IndexBufferID, zResID _ShaderBindingID, zU32 _uIndexFirst, zU32 _uIndexCount, zU32 _uVertexFirst, const zArrayBase<zenRes::zShaderResource>& _aResources)
	{
		static zenMem::zAllocatorPool sMemPool("Pool CreateMeshStrip", sizeof(ExportInfoGfxMeshStrip), 1, 5 );
		ExportInfoGfxMeshStrip* pExportInfo	= zenNew(&sMemPool) ExportInfoGfxMeshStrip;
		pExportInfo->mIndexBufferID				= _IndexBufferID;
		pExportInfo->mShaderBindingID			= _ShaderBindingID;
		pExportInfo->maResources				= _aResources;				
		pExportInfo->muVertexFirst				= _uVertexFirst;
		pExportInfo->muIndexFirst				= _uIndexFirst;
		pExportInfo->muIndexCount				= _uIndexCount;		
		return zcMgr::Export.CreateItem( zResID::kePlatformType_GFX, zenConst::keResType_GfxMeshStrip, pExportInfo );
	}


}

