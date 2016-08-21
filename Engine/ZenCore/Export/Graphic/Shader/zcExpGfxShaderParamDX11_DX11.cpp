#include "zcCore.h"

namespace zcExp
{

ExporterGfxShaderParamDX11_DX11::ExporterGfxShaderParamDX11_DX11(const ExportResultRef& _rExportOut)
: Super(_rExportOut.GetSafe())
, mrExport(_rExportOut)
{
	zenAssert(mrExport.IsValid());
}

bool ExporterGfxShaderParamDX11_DX11::ExportStart()
{
	ExportSkipWork();
	if( Super::ExportStart() )
	{
		ExportInfoGfxShaderParam* pExportInfo					= static_cast<ExportInfoGfxShaderParam*>(mpExportInfo);
		mrExport->mParentParamDefID								= pExportInfo->mParentParamDefID;	
		zEngineConstRef<ExportGfxShaderParamDefDX11> rParent	= zcDepot::ExportData.GetTyped<ExportGfxShaderParamDefDX11>(pExportInfo->mParentParamDefID);
		if( rParent.IsValid() )
		{
			mrExport->maParameterValues = rParent->maParameterDefaults;
			return TRUE;
		}
	}
	return FALSE;
}

}

