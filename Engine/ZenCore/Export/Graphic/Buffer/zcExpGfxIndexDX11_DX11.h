#pragma once

namespace zcExp
{	

class ExporterGfxIndexDX11_DX11 : public ExporterBase
{
zenClassDeclare(ExporterGfxIndexDX11_DX11, ExporterBase)
public:				
typedef zEngineRef<ExportGfxIndexDX11>	ExportDataRef; //todo use proper ref class
										ExporterGfxIndexDX11_DX11(const ExportDataRef& _rExportData);		
							
protected:	
	virtual bool						ExportWork(bool _bIsTHRTask);	
	ExportDataRef						mrExportData;
};

}
