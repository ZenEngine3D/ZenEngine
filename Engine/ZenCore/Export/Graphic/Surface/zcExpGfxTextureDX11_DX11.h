#pragma once

namespace zcExp
{	

class ExporterGfxTexture2DDX11_DX11 : public ExporterBase
{
zenClassDeclare(ExporterGfxTexture2DDX11_DX11, ExporterBase)
public:

typedef zEngineRef<ExportGfxTexture2D>	ExportResultRef; //todo use proper ref class
										ExporterGfxTexture2DDX11_DX11(const ExportResultRef& _rExportOut);
protected:	
	virtual bool				ExportWork(bool _bIsTHRTask);	
	ExportResultRef				mrExport;
};

}
