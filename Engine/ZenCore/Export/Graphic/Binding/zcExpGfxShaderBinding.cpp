#include "zcCore.h"

namespace zcExp
{
	zResID ExportInfoGfxShaderBinding::CallbackGetItemID(ePlatform _ePlatform, zenConst::eResType _eType, zenConst::eResSource _eSource, const zcExp::ExportInfoBase* _pExportInfo, bool& _bExistOut)
	{
		zenAssert(_eType==zenConst::keResType_GfxShaderBinding);
		zenAssert( _pExportInfo );
		const ExportInfoGfxShaderBinding* pExportInfo = static_cast<const ExportInfoGfxShaderBinding*>(_pExportInfo);

		zUInt uValidCount(0);
		zUInt uShaderCount = pExportInfo->maShaderID.Count();
		zArrayStatic<zU64> aSortedResname(uShaderCount);

		for(zUInt idx=0; idx<uShaderCount; ++idx)
			if( pExportInfo->maShaderID[idx].IsValid() )
				aSortedResname[uValidCount++] = pExportInfo->maShaderID[idx].GetHashID();

		aSortedResname.Sort();

		zResID::NameHash hName( aSortedResname.First(), aSortedResname.SizeMem() );
		return zcExp::ValidateItemID(_ePlatform, _eType, _eSource, hName, _bExistOut);
	}

	ExporterGfxShaderBinding::ExporterGfxShaderBinding(const zEngineRef<ExportGfxShaderBinding>& _rExportOut)
	: Super(_rExportOut.Get())
	, mrExport(_rExportOut)
	{
		zenStaticAssert( sizeof(ExportGfxShaderBinding::ShaderBindInfoIndex) <= sizeof(ExportGfxShaderBinding::ShaderBindInfoIndex::muResourceCollapsed) );
		zenAssert(_rExportOut.IsValid());
	}

	//=================================================================================================
	//! @brief		ShaderBinding pre-processing before export
	//! @details	Need to assign shader to their stage slot, make sure all needed ShaderParameter  
	//!				are provided, and store information on which shader stage they're used with
	//-------------------------------------------------------------------------------------------------
	//! @param _eRenderType		- Renderer type this resource will be for (DX9, DX11, GL, ...)
	//! @return 				- ResourceData of this resource type
	//=================================================================================================
	bool ExporterGfxShaderBinding::ExportStart()
	{
		if( !Super::ExportStart() )
			return false;
		
		ExportInfoGfxShaderBinding*	pExport = static_cast<ExportInfoGfxShaderBinding*>(mpExportInfo);
		const zResID aShaderIdnullptr[]={zResID(),zResID()};		
		zenStaticAssert(zenArrayCount(aShaderIdnullptr) == keShaderStage__Count);
		mrExport->maShaderID.Copy(aShaderIdnullptr, zenArrayCount(aShaderIdnullptr));					
		
		ExportGfxShaderBinding::ShaderBindInfoIndex DefaultResourceInfo;		
		zMap<ExportGfxShaderBinding::ShaderBindInfoIndex>::Key64 dCBufferParent;
		DefaultResourceInfo.muResourceCollapsed = 0xFFFFFFFFFFFFFFFF;		
		mrExport->mdResourceBind.SetDefaultValue(DefaultResourceInfo);
		mrExport->mdResourceBind.Init(16);				
		dCBufferParent.SetDefaultValue(DefaultResourceInfo);
		dCBufferParent.Init(16);
		
		//------------------------------------------------------------------------------------------
		// Find all resources used between all shader stages, and store how to reach them in each
		for(zUInt shaderIdx(0), shaderCount(pExport->maShaderID.Count()); shaderIdx<shaderCount; ++shaderIdx)
		{
			zEngineConstRef<zcExp::ExportGfxShader> rShader = zcDepot::ExportData.GetTyped<zcExp::ExportGfxShader>( pExport->maShaderID[shaderIdx] );
			if( rShader.IsValid() )
			{
				const eShaderStage ShaderStage		= rShader->meShaderStage;
				zenAssertMsg(!mrExport->maShaderID[ShaderStage].IsValid(), "Should only specify 1 shader per shader stage");	//! @todo Missing: error output, check computer vs vertex/pixel
				mrExport->maShaderID[ShaderStage]	= rShader->mResID;

				//---------------------------------------------------------------------
				// Find CBufferDef used by this shader stage					
				const zArrayStatic<zResID>&	aCBufferDef = rShader->maCBufferParentID;
				for( zUInt cbuffDefIdx(0), cbuffDefCount(aCBufferDef.Count()); cbuffDefIdx<cbuffDefCount; ++cbuffDefIdx )
				{
					const zResID CBuffResID = aCBufferDef[cbuffDefIdx];
					dCBufferParent.GetAdd(CBuffResID.GetHashID()).muShaderResIndex[ShaderStage] = rShader->maCBufferResIndex[cbuffDefIdx];
				}

				//---------------------------------------------------------------------
				// Find shader resources informations
				const zArrayStatic<ExportGfxShader::ShaderBindInfo>& aBindInfo = rShader->maResourceBinding;
				for(zU8 resIdx(0), resCount((zU8)aBindInfo.Count()); resIdx<resCount; ++resIdx)
				{
					const ExportGfxShader::ShaderBindInfo& ResBindInfo = aBindInfo[resIdx];
					mrExport->mdResourceBind.GetAdd(ResBindInfo.mzName.mhName).muShaderResIndex[ShaderStage] = resIdx;
				}
			}			
		}   

		//------------------------------------------------------------------------------------------
		// Map CBufferDef entries to their ResourceIndex in this Binding
		zUInt cbufIdx(0);
		mrExport->maCBufferParentID.SetCount( dCBufferParent.Count() );
		mrExport->maCBufferParentBindIndex.SetCount(dCBufferParent.Count());		
		mrExport->mdResourceBind.Export( mrExport->maResourceName, mrExport->maResourceBind );
		for( auto it(dCBufferParent.GetFirst());it.IsValid(); ++it, ++cbufIdx )
		{
			mrExport->maCBufferParentID[cbufIdx] = it.GetKey();
			for(zUInt resIdx(0), resCount(mrExport->maResourceBind.Count()); resIdx<resCount; ++resIdx)
			{
				if( mrExport->maResourceBind[resIdx].muResourceCollapsed == it.GetValue().muResourceCollapsed )
				{
					mrExport->maCBufferParentBindIndex[cbufIdx] = (zU8)resIdx;
					break;
				}
			}
		}

		//------------------------------------------------------------------------------------------
		// Process shader parameters binding infos (list param name in each bound ParamDef)
		mrExport->mdCBufferParamMask.Init( 64 );			
		mrExport->mdCBufferParamMask.SetDefaultValue(0);
		for(zUInt paramDefIdx(0), paramDefCount(mrExport->maCBufferParentID.Count()); paramDefIdx<paramDefCount; ++paramDefIdx )
		{
			zenAssert( paramDefIdx < mrExport->maCBufferParamMask.SizeItem() );			
			zEngineConstRef<ExportGfxCBufferDefinition> rParamDef = zcDepot::ExportData.GetTyped<ExportGfxCBufferDefinition>( mrExport->maCBufferParentID[paramDefIdx] );
			if( rParamDef.IsValid() )
			{
				for( auto it(rParamDef->mdParamInfo.GetFirst()); it.IsValid(); ++it)
					mrExport->mdCBufferParamMask.GetAdd(it.GetKey()) |= 1<<paramDefIdx;
			}
			else
			{
				//! @todo Missing: export error, missing ParamDef
			}
		}
		mrExport->mdCBufferParamMask.Export( mrExport->maCBufferParamName, mrExport->maCBufferParamMask );
		ExportSkipWork();
		return true;
	}
	
	//=================================================================================================
	//! @brief		Create a new Shader Binding 
	//! @details	The created resource will bind together shader stages(vertex/pixel/...) and 
	//!				a shader parameter instance
	//-------------------------------------------------------------------------------------------------
	//! @param _aShaderID		- Array of shader assigned to each shader stage
	//! @return 				- Created Resource
	//=================================================================================================
	zResID CreateGfxShaderBinding(const zArrayBase<zResID>& _aShaderID)
	{	
		//static zenMem::zAllocatorPool sMemPool("Pool CreateShaderBinding", sizeof(ExportInfoGfxShaderBinding), 1, 5 );
		ExportInfoGfxShaderBinding* pExportInfo	= zenNewPool ExportInfoGfxShaderBinding;
		pExportInfo->maShaderID					= _aShaderID;
		return zcMgr::Export.CreateItem( zResID::kePlatformType_GFX, zenConst::keResType_GfxShaderBinding, pExportInfo );
	}
	
}

