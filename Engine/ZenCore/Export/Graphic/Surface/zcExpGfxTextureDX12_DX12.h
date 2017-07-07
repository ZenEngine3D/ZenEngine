#pragma once

namespace zcExp
{	

class ExporterGfxTexture2DDX12_DX12 : public ExporterBase
{
zenClassDeclare(ExporterGfxTexture2DDX12_DX12, ExporterBase)
public:

typedef zEngineRef<ExportGfxTexture2D>	ExportResultRef; //todo use proper ref class
										ExporterGfxTexture2DDX12_DX12(const ExportResultRef& _rExportOut);
protected:	
	virtual bool						ExportWork(bool _bIsTHRTask);	
	ExportResultRef						mrExport;
};

}
