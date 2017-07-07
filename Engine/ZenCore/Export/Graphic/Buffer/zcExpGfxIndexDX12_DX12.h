#pragma once

namespace zcExp
{	

class ExporterGfxIndexDX12_DX12 : public ExporterBase
{
zenClassDeclare(ExporterGfxIndexDX12_DX12, ExporterBase)
public:				
typedef zEngineRef<ExportGfxIndexDX12>	ExportDataRef; //todo use proper ref class
										ExporterGfxIndexDX12_DX12(const ExportDataRef& _rExportData);		
							
protected:	
	virtual bool						ExportWork(bool _bIsTHRTask);	
	ExportDataRef						mrExportData;
};

}
