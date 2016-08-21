#include "zcCore.h"

namespace zcExp
{

zResID ExportInfoGfxShaderParamDef::CallbackGetItemID(zenConst::eResPlatform _ePlatform, zenConst::eResType _eType, zenConst::eResSource _eSource, const zcExp::ExportInfoBase* _pExportInfo, bool& _bExistOut)
{
	zenAssert(_ePlatform==zenConst::keResPlatform_DX11 && _eType==zenConst::keResType_GfxShaderParamDef);
	zenAssert( _pExportInfo );
	const ExportInfoGfxShaderParamDef*		pExportInfo		= static_cast<const ExportInfoGfxShaderParamDef*>(_pExportInfo);
	zEngineConstRef<ExportGfxShaderDX11>	rParentShader	= zcDepot::ExportData.GetTyped<ExportGfxShaderDX11>( pExportInfo->mParentShaderID );	
	if( rParentShader.IsValid() && rParentShader->maParamDefID.Count() > (zUInt)pExportInfo->meBufferIndex )
	{
		zResID pPreProcessId = rParentShader->maParamDefID[pExportInfo->meBufferIndex]; //Re-use already processed ID (from shader creation)		
		zResID::NameHash _hName = pPreProcessId.GetName();
		zResID newResID(_hName, _ePlatform, _eType, _eSource);
		zEngineConstRef<zenRes::zExportData> rExportData	= zcDepot::ExportData.GetAnySource( newResID );
		_bExistOut											= rExportData.IsValid();
		return _bExistOut ? rExportData->mResID : newResID;
	}
	return zResID();		
}

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
	return zcMgr::Export.CreateItem( zResID::kePlatformType_GFX, zenConst::keResType_GfxShaderParamDef, pExportInfo );
}

}

