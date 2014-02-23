#include "libZenCore.h"
#if AW_EXPORT_OR_RESOURCE_DX11

namespace EExp
{

bool SerialGfxShaderParam_DX11::Serialize( EExp::Serializer_Base& _Serializer )
{
	return false;
}

bool SerialGfxShaderParam_DX11::ExportWork(bool _bIsTHRTask)
{
	ExportInfo* pExportInfo		= static_cast<ExportInfo*>(mpExportInfo);
	mSerial.mParentParamDefID	= pExportInfo->mParentParamDefID;	
	return TRUE;
}

bool SerialGfxShaderParam_DX11::ExportEnd()
{
	ExportInfo* pExportInfo						= static_cast<ExportInfo*>(mpExportInfo);
	EExp::SerialGfxShaderParamDef_DX11* pParent	= EMgr::SerialItems.GetItem<EExp::SerialGfxShaderParamDef_DX11>(pExportInfo->mParentParamDefID);
	if( !pParent )
		return FALSE;
	
	maParameterValues							= pParent->maParameterDefaults;
	if( pExportInfo->maParamValues.Count() )
	{
		const EExp::ParameterBase** ppParamCur	= pExportInfo->maParamValues.First();
		const EExp::ParameterBase** ppParamLast	= pExportInfo->maParamValues.Last();
		while( ppParamCur && ppParamCur <= ppParamLast )
		{
			ShaderParamItemInfo ItemInfo;
			if( pParent->mdParameters.Get( (*ppParamCur)->mhName, ItemInfo ) )
				zenMem::Copy( &maParameterValues[ItemInfo.muOffset], (*ppParamCur)->GetData(), zenMath::Min( (*ppParamCur)->muSizeTotal, ItemInfo.muSize) );
			++ppParamCur;
		}
	}
	return Super::ExportEnd();	
}

}

#endif //AW_EXPORT_OR_RESOURCE_DX11
