#include "zcCore.h"

namespace zcExp
{
	zResID ExportInfoGfxShaderBinding::CallbackGetItemID(zenConst::eResPlatform _ePlatform, zenConst::eResType _eType, zenConst::eResSource _eSource, const zcExp::ExportInfoBase* _pExportInfo, bool& _bExistOut)
	{
		ZENAssert(_ePlatform==zenConst::keResPlatform_DX11 && _eType==zenConst::keResType_GfxShaderBinding);
		ZENAssert( _pExportInfo );
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

	ExporterGfxShaderBinding::ExporterGfxShaderBinding(const ResDataRef& _rResData)
	: Super(_rResData)
	{
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

		//---------------------------------------------------------------------
		// Set Shader used for each stage, and needed ShaderParam for each
		//---------------------------------------------------------------------	
		ExportInfoGfxShaderBinding*	pExport = static_cast<ExportInfoGfxShaderBinding*>(mpExportInfo);
		const zResID					aShaderIdnullptr[]={zResID(),zResID()};
		zArrayStatic<zResID>			aShaderID;
		ZENStaticAssert(ZENArrayCount(aShaderIdnullptr) == zenConst::keShaderStage__Count);
		aShaderID.Copy(aShaderIdnullptr, ZENArrayCount(aShaderIdnullptr));
		mdStagePerParamDef.Init(8);
		mdStagePerParamDef.SetDefaultValue(0);
		for(zUInt idx=0; idx<pExport->maShaderID.Count(); ++idx)
		{
			zEngineConstRef<zcRes::GfxShaderResData> rShader = zcDepot::ResourceData.GetItem<zcRes::GfxShaderResData>( pExport->maShaderID[idx] );
			if( rShader.IsValid() )
			{
				ZENAssertMsg(!aShaderID[rShader->meShaderStage].IsValid(), "Should only specify 1 shader per shader stage");	//! @todo Missing: error output
				aShaderID[rShader->meShaderStage] = rShader->mResID;
				for(const zResID *pParamIDCur(rShader->maParamDefID.First()), *pParamIDLast(rShader->maParamDefID.Last()); pParamIDCur<=pParamIDLast;  ++pParamIDCur )
				{
					if( pParamIDCur->IsValid() )
						mdStagePerParamDef.GetAdd(pParamIDCur->GetHashID()) |= 1<<rShader->meShaderStage;
				}					
			}			
		}
		pExport->maShaderID = aShaderID;
		return true;
	}
	
	//=================================================================================================
	//! @brief		Create a new Shader Binding 
	//! @details	The created resource will bind together shader stages(vertex/pixel/...) and 
	//!				a shader parameter instance
	//-------------------------------------------------------------------------------------------------
	//! @param _aShaderID		- Array of shader assigned to each shader stage
	//! @param _aShaderParamID	- Array of Constant buffer to tie to each shader
	//! @param _aTexture		- Array of texture binding definition tied to each shader
	//! @return 				- Created Resource
	//=================================================================================================
	zResID CreateGfxShaderBinding(const zArrayBase<zResID>& _aShaderID/*, const Array<zResID>& _aShaderParamID, const Array<zcExp::TextureBinding>& _aTexture*/)
	{	
		static zenMem::zAllocatorPool sMemPool("Pool CreateShaderBinding", sizeof(ExportInfoGfxShaderBinding), 1, 5 );
		ExportInfoGfxShaderBinding* pExportInfo	= zenNew(&sMemPool) ExportInfoGfxShaderBinding;
		pExportInfo->maShaderID					= _aShaderID;
		return zcMgr::Export.CreateItem( zResID::kePlatformType_GFX, zenConst::keResType_GfxShaderBinding, pExportInfo );
	}
	
}

