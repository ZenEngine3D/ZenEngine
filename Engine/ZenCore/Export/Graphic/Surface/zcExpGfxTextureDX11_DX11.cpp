#include "zcCore.h"
#include "zcExpGfxTexture_PC.h"

namespace zcExp
{

ExporterGfxTexture2dDX11_DX11::ExporterGfxTexture2dDX11_DX11(const ExportResultRef& _rExportOut)
: ExporterBase(_rExportOut.GetSafe())
, mrExport(_rExportOut)
{
	zenAssert(mrExport.IsValid());
}

bool ExporterGfxTexture2dDX11_DX11::ExportWork(bool _bIsTHRTask)
{
	ExportInfoGfxTexture2d* pExportInfo	= static_cast<ExportInfoGfxTexture2d*>(mpExportInfo);		
	mrExport->meFormat					= pExportInfo->meFormatOutput;
	mrExport->mCreationFlags			= pExportInfo->mCreationFlags;
	return GenerateTexture(mrExport->maMipData, *pExportInfo);
}

}

