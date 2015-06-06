#include "zcCore.h"

namespace zcExp
{

#if 0 //SF replaced
zResID ExportInfoGfxShaderParamDef::CallbackGetItemID(zenConst::eResPlatform _ePlatform, zenConst::eResType _eType, zenConst::eResSource _eSource, const zcExp::ExportInfoBase* _pExportInfo, bool& _bExistOut)
{
	ZENAssert(_ePlatform==zenConst::keResPlatform_DX11 && _eType==zenConst::keResType_GfxShaderParamDef);
	ZENAssert( _pExportInfo );
	const ExportInfo*			pExportInfo		= static_cast<const ExportInfo*>(_pExportInfo);
	const SerialShader_DX11*	pParentShader	= EMgr::SerialItems.GetItem<SerialShader_DX11>( pExportInfo->mParentShaderID );	
	if( pParentShader && pParentShader->maParamDefID.Count() > (zUInt)pExportInfo->meBufferIndex )
	{
		zResID pPreProcessId = pParentShader->maParamDefID[pExportInfo->meBufferIndex]; //Re-use already processed ID (from shader creation)
		return zcExp::ValidateItemID(_ePlatform, _eType, _eSource, pPreProcessId.Name(), _bExistOut);
	}
	return zResID();		
}
#else
zResID ExportInfoGfxShaderParamDef::CallbackGetItemID(zenConst::eResPlatform _ePlatform, zenConst::eResType _eType, zenConst::eResSource _eSource, const zcExp::ExportInfoBase* _pExportInfo, bool& _bExistOut)
{
	ZENAssert(_ePlatform==zenConst::keResPlatform_DX11 && _eType==zenConst::keResType_GfxShaderParamDef);
	ZENAssert( _pExportInfo );
	const ExportInfoGfxShaderParamDef*	pExportInfo		= static_cast<const ExportInfoGfxShaderParamDef*>(_pExportInfo);
	const ExportDataGfxShaderDX11*			pParentShader	= EMgr::SerialItems.GetItem<ExportDataGfxShaderDX11>( pExportInfo->mParentShaderID );	
	if( pParentShader && pParentShader->maParamDefID.Count() > (zUInt)pExportInfo->meBufferIndex )
	{
		zResID pPreProcessId = pParentShader->maParamDefID[pExportInfo->meBufferIndex]; //Re-use already processed ID (from shader creation)
		//return zcExp::ValidateItemID(_ePlatform, _eType, _eSource, pPreProcessId.Name(), _bExistOut);
		//ValidateItemID(zenConst::eResPlatform _ePlatform, zenConst::eResType _eType, zenConst::eResSource _eSource, zResID::NameHash _hName, bool& _bExistOut)
		{
			zResID::NameHash _hName = pPreProcessId.Name();
			zResID newResID(_hName, _ePlatform, _eType, _eSource);
			zcExp::SerialItem* pItem	= EMgr::SerialItems.GetItemBaseAnySource( newResID );
			_bExistOut					= pItem != NULL;
			return _bExistOut ? pItem->mResID : newResID;
		}
	}
	return zResID();		
}
#endif

//=================================================================================================
//! @brief		Create a new Shader Parameter Definition
//! @details	
//-------------------------------------------------------------------------------------------------
//! @param _ParentShaderID	- ResousrceID of shader to extract Constant buffer Infos
//! @param _eBufferIndex	- Which Constant Buffer to export
//! @return 				- Unique zResID of created Resource
//=================================================================================================
zResID CreateGfxShaderParamDef( zResID _ParentShaderID, zcExp::eShaderParamFreq _eBufferIndex )
{
	static zenMem::zAllocatorPool sMemPool("Pool CreateShaderParamDef", sizeof(ExportInfoGfxShaderParamDef), 1, 5 );
	ExportInfoGfxShaderParamDef* pExportInfo	= zenNew(&sMemPool) ExportInfoGfxShaderParamDef;	
	pExportInfo->mParentShaderID				= _ParentShaderID;
	pExportInfo->meBufferIndex					= _eBufferIndex;		
	return EMgr::Export.CreateItem( zResID::kePlatformType_GFX, zenConst::keResType_GfxShaderParamDef, pExportInfo );
}

}

