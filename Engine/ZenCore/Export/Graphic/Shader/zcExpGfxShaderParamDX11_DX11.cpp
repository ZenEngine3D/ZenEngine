#include "zcCore.h"

namespace zcExp
{

ExporterGfxShaderParamDX11_DX11::ExporterGfxShaderParamDX11_DX11(const ExportDataRef& _rExportData)
: Super(_rExportData.GetSafe())
, mrExportData(_rExportData)
{
}

bool ExporterGfxShaderParamDX11_DX11::ExportStart()
{
	ExportSkipWork();
	if( Super::ExportStart() )
	{
		ExportInfoGfxShaderParam* pExportInfo		= static_cast<ExportInfoGfxShaderParam*>(mpExportInfo);
		mrExportData->mParentParamDefID				= pExportInfo->mParentParamDefID;	
		ExportDataGfxShaderParamDefDX11* pParent	= EMgr::SerialItems.GetItem<ExportDataGfxShaderParamDefDX11>(pExportInfo->mParentParamDefID);
		if( pParent )
		{
			mrExportData->maParameterValues			= pParent->maParameterDefaults;
			return TRUE;
		}
	}
	return FALSE;
}

}

