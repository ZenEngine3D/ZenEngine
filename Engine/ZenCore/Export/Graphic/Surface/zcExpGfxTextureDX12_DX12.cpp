#include "zcCore.h"
#include "zcExpGfxTexture_PC.h"

namespace zcExp
{

ExporterGfxTexture2DDX12_DX12::ExporterGfxTexture2DDX12_DX12(const ExportResultRef& _rExportOut)
: ExporterBase(_rExportOut.GetSafe())
, mrExport(_rExportOut)
{
	zenAssert(mrExport.IsValid());
}

bool ExporterGfxTexture2DDX12_DX12::ExportWork(bool _bIsTHRTask)
{
	ExportInfoGfxTexture2D* pExportInfo	= static_cast<ExportInfoGfxTexture2D*>(mpExportInfo);		
	mrExport->meFormat					= pExportInfo->meFormatOutput;
	mrExport->mCreationFlags			= pExportInfo->mCreationFlags;
	return GenerateTexture(mrExport->maMipData, *pExportInfo);
}

}
