#include "zcCore.h"

namespace zcExp
{

ExporterGfxShaderParamDX11_DX11::ExporterGfxShaderParamDX11_DX11(const ResDataRef& _rResData)
: Super(_rResData.GetSafe())
, mrResData(_rResData)
{
}

bool ExporterGfxShaderParamDX11_DX11::ExportStart()
{
	ExportSkipWork();
	if( Super::ExportStart() )
	{
		ExportInfoGfxShaderParam* pExportInfo					= static_cast<ExportInfoGfxShaderParam*>(mpExportInfo);
		mrResData->mParentParamDefID							= pExportInfo->mParentParamDefID;	
		zEngineConstRef<ResDataGfxShaderParamDefDX11> rParent	= zcDepot::ResourceData.GetItem<ResDataGfxShaderParamDefDX11>(pExportInfo->mParentParamDefID);
		if( rParent.IsValid() )
		{
			mrResData->maParameterValues = rParent->maParameterDefaults;
			return TRUE;
		}
	}
	return FALSE;
}

}

