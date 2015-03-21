#include "zcCore.h"
#if ZEN_EXPORT_OR_RESOURCE_DX11

namespace zcExp
{

bool SerialGfxShaderParam_DX11::Serialize( zcExp::Serializer_Base& _Serializer )
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
	zcExp::SerialGfxShaderParamDef_DX11* pParent	= EMgr::SerialItems.GetItem<zcExp::SerialGfxShaderParamDef_DX11>(pExportInfo->mParentParamDefID);
	if( !pParent )
		return FALSE;
	
	maParameterValues							= pParent->maParameterDefaults;
	if( pExportInfo->maParamValues.Count() )
	{
		const zcExp::ParameterBase** ppParamCur	= pExportInfo->maParamValues.First();
		const zcExp::ParameterBase** ppParamLast	= pExportInfo->maParamValues.Last();
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

#endif //ZEN_EXPORT_OR_RESOURCE_DX11
