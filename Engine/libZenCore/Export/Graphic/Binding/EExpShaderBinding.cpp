#include "libZenCore.h"

namespace EExp
{
	zResID SerialShaderBinding_Base::CallbackGetItemID(zenConst::eResPlatform _ePlatform, zenConst::eResType _eType, zenConst::eResSource _eSource, const EExp::ExportInfoBase* _pExportInfo, bool& _bExistOut)
	{
		ZENAssert(_ePlatform==zenConst::keResPlatform_DX11 && _eType==zenConst::keResType_GfxShaderBinding);
		ZENAssert( _pExportInfo );
		const ExportInfo* pExportInfo = static_cast<const ExportInfo*>(_pExportInfo);

		zUInt uValidCount(0);
		zUInt uShaderCount = pExportInfo->maShaderID.Count();
		zArrayStatic<zU64> aSortedResname(uShaderCount);

		for(zUInt idx=0; idx<uShaderCount; ++idx)
			if( pExportInfo->maShaderID[idx].IsValid() )
				aSortedResname[uValidCount++] = pExportInfo->maShaderID[idx].HashID();

		aSortedResname.Sort();

		zResID::NameHash hName( aSortedResname.First(), aSortedResname.Size() );
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
	zResID CreateGfxShaderBinding(const zArrayBase<zResID>& _aShaderID/*, const Array<zResID>& _aShaderParamID, const Array<EExp::TextureBinding>& _aTexture*/)
	{	
		static zenMem::zAllocatorPool sMemPool("Pool CreateShaderBinding", sizeof(SerialShaderBinding_Base::ExportInfo), 1, 5 );
		SerialShaderBinding_Base::ExportInfo* pExportInfo	= zenNew(&sMemPool) SerialShaderBinding_Base::ExportInfo;
		pExportInfo->maShaderID								= _aShaderID;
		return EMgr::Export.CreateItem( zResID::kePlatformType_GFX, zenConst::keResType_GfxShaderBinding, pExportInfo );
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
		const zResID						aShaderIdNull[]={zResID(),zResID()};
		zArrayStatic<zResID>					aShaderID;
		ZENStaticAssert(ZENArrayCount(aShaderIdNull) == keShaderStage__Count);
		aShaderID.Copy(aShaderIdNull, ZENArrayCount(aShaderIdNull));
		pExport->mdStagePerParamDef.Init(8);
		pExport->mdStagePerParamDef.SetDefaultValue(0);
		for(zUInt idx=0; idx<pExport->maShaderID.Count(); ++idx)
		{
			SerialShader_Base* pShader = EMgr::SerialItems.GetItem<SerialShader_Base>( pExport->maShaderID[idx] );
			if( pShader )
			{
				ZENAssertMsg(!aShaderID[pShader->meShaderStage].IsValid(), "Should only specify 1 shader per shader stage");	//! @todo error output
				aShaderID[pShader->meShaderStage] = pShader->mResID;
				for(zResID *pParamIDCur(pShader->maParamDefID.First()), *pParamIDLast(pShader->maParamDefID.Last()); pParamIDCur<=pParamIDLast;  ++pParamIDCur )
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

