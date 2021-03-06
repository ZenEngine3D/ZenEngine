#include "zcCore.h"

namespace zcExp
{

//! @todo Clean move all output parameters first
zResID ExportInfoGfxCBufferDefinition::CallbackGetItemID(ePlatform _ePlatform, zenConst::eResType _eType, zenConst::eResSource _eSource, const zcExp::ExportInfoBase* _pExportInfo, bool& _bExistOut)
{
	zenAssert(_eType==zenConst::keResType_GfxCBufferDefinition);
	zenAssert( _pExportInfo );
	zResID::NameHash						hResName;
	const ExportInfoGfxCBufferDefinition*	pExportInfo = static_cast<const ExportInfoGfxCBufferDefinition*>(_pExportInfo);
	const zArray<zHash32>&				aParamName	= pExportInfo->maParamName;
	const zArray<GfxCBufferParamInfo>&	aParamInfo	= pExportInfo->maParamInfo;

	for(zUInt itemIdx(0), itemCount(aParamName.size()); itemIdx<itemCount; ++itemIdx)
	{		
		hResName.Append( (void*)&aParamName[itemIdx], sizeof(aParamName[itemIdx]) );
		hResName.Append( (void*)&aParamInfo[itemIdx], sizeof(aParamInfo[itemIdx]) );
	}

	zResID newResID(hResName, _ePlatform, _eType, _eSource);
	zEngineConstRef<zenRes::zExportData> rExportData	= zcDepot::ExportData.GetAnySource( newResID );
	_bExistOut											= rExportData.IsValid();
	return _bExistOut ? rExportData->mResID : newResID;
}

ExporterGfxShaderParamDef::ExporterGfxShaderParamDef(const ExportResultRef& _rExportOut)
: Super(_rExportOut.Get())
, mrExport(_rExportOut)
{
	zenAssert(mrExport.IsValid());
}

bool ExporterGfxShaderParamDef::ExportStart()
{
	if( !Super::ExportStart() )
		return false;

	zU32 uMaxSize(0);
	ExportInfoGfxCBufferDefinition*			pExportInfo = static_cast<ExportInfoGfxCBufferDefinition*>(mpExportInfo);
	zMap<GfxCBufferParamInfo>::Key32&		dParamInfo	= mrExport->mdParamInfo;
	const zArrayDyn<zHash32>&				aParamName	= pExportInfo->maParamName;
	const zArrayDyn<GfxCBufferParamInfo>&	aParamInfo	= pExportInfo->maParamInfo;
	dParamInfo.Init( static_cast<zU32>(aParamName.size()) );
	
	for(zUInt idx(0), count(aParamName.size()); idx<count; ++idx)
	{
		dParamInfo.GetAdd( aParamName[idx] )	= aParamInfo[idx];
		uMaxSize								= zenMath::Max<zU32>(uMaxSize, aParamInfo[idx].muOffset+aParamInfo[idx].muSize);
	}

	zenAssert( uMaxSize <= pExportInfo->maDefaultValues.SizeMem() );
	mrExport->muDefaultValues	= pExportInfo->maDefaultValues;
	mrExport->mzBufferName		= pExportInfo->mzBufferName;
	ExportSkipWork();
	return true;
}

//=================================================================================================
//! @brief		Create a new Shader Parameter Definition
//! @details	
//-------------------------------------------------------------------------------------------------
//! @param _aParamName		- List of parameters hashed name in this CBuffer Definition
//! @param _aParamInfo		- List of parameters informations in this CBuffer Definition
//! @param _uDefaultValues	- Memory with default parameters values of this CBuffer Definition
//! @return 				- Unique zResID of created Resource
//=================================================================================================
zResID CreateGfxCBufferDefinition( const zStringHash32& _zBufferName, const zArray<zHash32>& _aParamName, const zArray<GfxCBufferParamInfo>& _aParamInfo, const zArray<zU8>& _uDefaultValues )
{
	auto* pExportInfo				= zenMem::NewPool<ExportInfoGfxCBufferDefinition>();
	pExportInfo->mzBufferName		= _zBufferName;
	pExportInfo->maParamName		= _aParamName;
	pExportInfo->maParamInfo		= _aParamInfo;
	pExportInfo->maDefaultValues	= _uDefaultValues;		
	return zcMgr::Export.CreateItem( zResID::kePlatformType_GFX, zenConst::keResType_GfxCBufferDefinition, pExportInfo );
}

}