#include "libZenCore.h"

namespace EExp
{
	zenResID SerialShaderBinding_Base::CallbackGetItemID(awconst::eResPlatform _ePlatform, awconst::eResType _eType, awconst::eResSource _eSource, const EExp::ExportInfoBase* _pExportInfo, bool& _bExistOut)
	{
		AWAssert(_ePlatform==awconst::keResPlatform_DX11 && _eType==awconst::keResType_GfxShaderBinding);
		AWAssert( _pExportInfo );
		const ExportInfo* pExportInfo = static_cast<const ExportInfo*>(_pExportInfo);

		zenUInt uValidCount(0);
		zenUInt uShaderCount = pExportInfo->maShaderID.Count();
		zenArrayStatic<zenU64> aSortedResname(uShaderCount);

		for(zenUInt idx=0; idx<uShaderCount; ++idx)
			if( pExportInfo->maShaderID[idx].IsValid() )
				aSortedResname[uValidCount++] = pExportInfo->maShaderID[idx].HashID();

		aSortedResname.Sort();

		zenResID::NameHash hName( aSortedResname.First(), aSortedResname.Size() );
		return EExp::ValidateItemID(_ePlatform, _eType, _eSource, hName, _bExistOut);
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
	zenResID CreateGfxShaderBinding(const zenArrayBase<zenResID>& _aShaderID/*, const Array<zenResID>& _aShaderParamID, const Array<EExp::TextureBinding>& _aTexture*/)
	{	
		static zbMem::PoolAllocator sMemPool("Pool CreateShaderBinding", sizeof(SerialShaderBinding_Base::ExportInfo), 1, 5 );
		SerialShaderBinding_Base::ExportInfo* pExportInfo	= zenNew(&sMemPool) SerialShaderBinding_Base::ExportInfo;
		pExportInfo->maShaderID								= _aShaderID;
		return EMgr::Export.CreateItem( zenResID::kePlatformType_GFX, awconst::keResType_GfxShaderBinding, pExportInfo );
	}

	//=================================================================================================
	//! @brief		ShaderBinding pre-processing before export
	//! @details	Need to assign shader to their stage slot, make sure all needed ShaderParameter  
	//!				are provided, and store information on which shader stage they're used with
	//-------------------------------------------------------------------------------------------------
	//! @param _eRenderType		- Renderer type this resource will be for (DX9, DX11, GL, ...)
	//! @return 				- SerialItem of this resource type
	//=================================================================================================
	bool SerialShaderBinding_Base::ExportStart()
	{
		if( !Super::ExportStart() )
			return false;

		//---------------------------------------------------------------------
		// Set Shader used for each stage, and needed ShaderParam for each
		//---------------------------------------------------------------------	
		SerialShaderBinding_Base::ExportInfo*	pExport = static_cast<SerialShaderBinding_Base::ExportInfo*>(mpExportInfo);
		const zenResID						aShaderIdNull[]={zenResID(),zenResID()};
		zenArrayStatic<zenResID>					aShaderID;
		AWStaticAssert(AWArrayCount(aShaderIdNull) == keShaderStage__Count);
		aShaderID.Copy(aShaderIdNull, AWArrayCount(aShaderIdNull));
		pExport->mdStagePerParamDef.Init(8);
		pExport->mdStagePerParamDef.SetDefaultValue(0);
		for(zenUInt idx=0; idx<pExport->maShaderID.Count(); ++idx)
		{
			SerialShader_Base* pShader = EMgr::SerialItems.GetItem<SerialShader_Base>( pExport->maShaderID[idx] );
			if( pShader )
			{
				AWAssertMsg(!aShaderID[pShader->meShaderStage].IsValid(), "Should only specify 1 shader per shader stage");	//! @todo error output
				aShaderID[pShader->meShaderStage] = pShader->mResID;
				for(zenResID *pParamIDCur(pShader->maParamDefID.First()), *pParamIDLast(pShader->maParamDefID.Last()); pParamIDCur<=pParamIDLast;  ++pParamIDCur )
				{
					if( pParamIDCur->IsValid() )
						pExport->mdStagePerParamDef.GetAdd(pParamIDCur->HashID()) |= 1<<pShader->meShaderStage;
				}					
			}			
		}
		pExport->maShaderID = aShaderID;
		return true;
	}
}

